#pragma once

#include <cstdint>
#include <stdbool.h>

// Struct to hold all physical hardware inputs on the wheel
struct SteeringInputs 
{
    // 5 Pushbuttons
    bool btn_1;
    bool btn_2;
    bool btn_4;
    bool btn_5;
    bool btn_6;

    // Toggle Switch
    bool toggle_up;

    // Rotary Encoder Pushbuttons
    bool bottom_rotary_btn;
    bool top_rotary_btn;

    // Rotary Encoder Positions (Counted values)
    int32_t bottom_encoder_count;
    int32_t top_encoder_count;

    // Multiposition Switch (The 4-bit BCD value 0-9)
    uint8_t multi_position;
};

// Main state object shared between tasks
struct WheelState 
{
    SteeringInputs current_inputs;
    
    // System Metadata
    uint32_t last_update_time;
    bool can_bus_active;
};