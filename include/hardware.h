// hardware.h
#pragma once

#include "can.h"
#include "gpio.h"
#include "steering_state.h"
#include <zephyr/device.h>
#include <zephyr/devicetree.h>

class Hardware
{
  public:
    Hardware(WheelState *state);

    // 5 Pushbuttons
    GpioPin btn_1;
    GpioPin btn_2;
    GpioPin btn_4;
    GpioPin btn_5;
    GpioPin btn_6;

    // Toggle Switch
    GpioPin toggle_up;

    // Rotary Encoder Pushbuttons
    GpioPin bottom_rotary_btn;
    GpioPin top_rotary_btn;

    // Rotary Encoder A/B Channels (Digital Inputs)
    GpioPin bottom_enc_a;
    GpioPin bottom_enc_b;
    GpioPin top_enc_a;
    GpioPin top_enc_b;

    // Multiposition Switch Bits (4-bit BCD)
    GpioPin multi_bit1;
    GpioPin multi_bit2;
    GpioPin multi_bit4;
    GpioPin multi_bit8;

    // Status LEDs
    GpioPin mcu_err_led;
    GpioPin mcu_stat_led;

    // Single CAN Bus (FDCAN1)
    CanBus can1;

    int init();

  private:
    WheelState *wheel; // Replaced 'vehicle' pointer

    // GPIO Port device pointers needed for Zephyr initialization
    const struct device *gpioa_ = nullptr;
    const struct device *gpiob_ = nullptr;
    const struct device *gpioc_ = nullptr;
    const struct device *gpiod_ = nullptr;
    const struct device *gpioe_ = nullptr;
    
    // CAN device pointer
    const struct device *can1_dev = nullptr;

    // Internal initialization routines
    int initializeGPIOs();
    int initializeCAN();
};