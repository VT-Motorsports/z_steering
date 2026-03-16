#include "steering_state.h"
#include "hardware.h"
#include "periodic_task.h"
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(steering_task, LOG_LEVEL_INF);

class SteeringInputTask : public PeriodicTask<SteeringInputTask> {
    friend class PeriodicTask<SteeringInputTask>;

public:
    void set_wheel_state(WheelState *w) { wheel_ = w; }
    void set_hardware(Hardware *hw)   { hw_ = hw; }

private:
    WheelState *wheel_ = nullptr;
    Hardware   *hw_    = nullptr;
    
    // Previous states for quadrature decoding
    bool last_top_enc_a = false;
    bool last_bot_enc_a = false;

    // BCD switches are usually active low (GND = active)
    uint8_t read_bcd_switch() {
        bool b1, b2, b4, b8;
        hw_->multi_bit1.get(&b1);
        hw_->multi_bit2.get(&b2);
        hw_->multi_bit4.get(&b4);
        hw_->multi_bit8.get(&b8);

        uint8_t val = (!b1 ? 1 : 0) | (!b2 ? 2 : 0) | (!b4 ? 4 : 0) | (!b8 ? 8 : 0);
        return val;
    }

    void handle_encoders() {
        bool top_a, top_b, bot_a, bot_b;
        hw_->top_enc_a.get(&top_a);
        hw_->top_enc_b.get(&top_b);
        hw_->bottom_enc_a.get(&bot_a);
        hw_->bottom_enc_b.get(&bot_b);

        // Top Encoder Logic
        if (top_a != last_top_enc_a && top_a) { // Rising edge on A
            if (!top_b) wheel_->current_inputs.top_encoder_count++;
            else wheel_->current_inputs.top_encoder_count--;
        }
        last_top_enc_a = top_a;

        // Bottom Encoder Logic
        if (bot_a != last_bot_enc_a && bot_a) { // Rising edge on A
            if (!bot_b) wheel_->current_inputs.bottom_encoder_count++;
            else wheel_->current_inputs.bottom_encoder_count--;
        }
        last_bot_enc_a = bot_a;
    }

    void run() {
        if (!wheel_ || !hw_) return;

        // 1. Read Pushbuttons (Assumes !get for Active Low)
        bool raw;
        hw_->btn_1.get(&raw); wheel_->current_inputs.btn_1 = !raw;
        hw_->btn_2.get(&raw); wheel_->current_inputs.btn_2 = !raw;
        hw_->btn_4.get(&raw); wheel_->current_inputs.btn_4 = !raw;
        hw_->btn_5.get(&raw); wheel_->current_inputs.btn_5 = !raw;
        hw_->btn_6.get(&raw); wheel_->current_inputs.btn_6 = !raw;
        hw_->toggle_up.get(&raw); wheel_->current_inputs.toggle_up = !raw;

        // 2. Read Encoders and BCD
        handle_encoders();
        wheel_->current_inputs.multi_position = read_bcd_switch();

        // 3. Construct and Send CAN Frame
        struct can_frame frame = {0};
        frame.id = 0x150; // Standard Wheel ID
        frame.dlc = 8;
        
        // Byte 0: Pushbuttons
        frame.data[0] = (wheel_->current_inputs.btn_1 << 0) | 
                        (wheel_->current_inputs.btn_2 << 1) |
                        (wheel_->current_inputs.btn_4 << 2) |
                        (wheel_->current_inputs.btn_5 << 3) |
                        (wheel_->current_inputs.btn_6 << 4);

        // Byte 1: BCD + Toggle
        frame.data[1] = (wheel_->current_inputs.multi_position & 0x0F) |
                        (wheel_->current_inputs.toggle_up << 4);

        // Bytes 2-5: Encoder Counts (Big Endian)
        uint16_t top = (uint16_t)wheel_->current_inputs.top_encoder_count;
        uint16_t bot = (uint16_t)wheel_->current_inputs.bottom_encoder_count;
        
        frame.data[2] = (top >> 8); frame.data[3] = (top & 0xFF);
        frame.data[4] = (bot >> 8); frame.data[5] = (bot & 0xFF);
        
        hw_->can1.send(&frame, K_NO_WAIT);
        
        wheel_->last_update_time = k_uptime_get_32();
    }
};

static SteeringInputTask input_task_instance;
K_THREAD_STACK_DEFINE(input_task_stack, 2048);

void start_steering_input_task(WheelState *w, Hardware *hw) {
    input_task_instance.set_wheel_state(w);
    input_task_instance.set_hardware(hw);
    input_task_instance.start(input_task_stack, K_THREAD_STACK_SIZEOF(input_task_stack), 20, 5, w);
}