// hardware.cpp
#include "hardware.h"
#include "steering_state.h"
#include "zephyr/drivers/can.h"
#include <cstdint>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(hardware);

int Hardware::init()
{
    LOG_INF("Initializing Wheel hardware...");

    
    if (initializeGPIOs() != 0)
    {
        LOG_ERR("Failed to initialize Wheel GPIOs");
        return -1;
    }

    if (initializeCAN() != 0) 
    {
        LOG_ERR("Failed to initialize CAN");
        return -2;
    }

    LOG_INF("Wheel Hardware initialized successfully");
    return 0;
}

Hardware::Hardware(WheelState *state) : can1(state), wheel(state)
{
}

int Hardware::initializeGPIOs()
{
    // Get GPIO ports from Devicetree
    gpioa_ = DEVICE_DT_GET(DT_NODELABEL(gpioa));
    gpiob_ = DEVICE_DT_GET(DT_NODELABEL(gpiob));
    gpioc_ = DEVICE_DT_GET(DT_NODELABEL(gpioc));
    gpiod_ = DEVICE_DT_GET(DT_NODELABEL(gpiod));
    gpioe_ = DEVICE_DT_GET(DT_NODELABEL(gpioe));

    if (!gpioa_ || !gpiob_ || !gpioc_ || !gpiod_ || !gpioe_)
    {
        LOG_ERR("Failed to get one or more GPIO ports");
        return -1;
    }

    // --- 5 Pushbuttons ---
    btn_1.init(gpioe_, 6, GPIO_INPUT | GPIO_PULL_UP);
    btn_2.init(gpioe_, 3, GPIO_INPUT | GPIO_PULL_UP);
    btn_4.init(gpiob_, 15, GPIO_INPUT | GPIO_PULL_UP);
    btn_5.init(gpioe_, 4, GPIO_INPUT | GPIO_PULL_UP);
    btn_6.init(gpiod_, 6, GPIO_INPUT | GPIO_PULL_UP);

    // --- Toggle Switch (Flick Up) ---
    toggle_up.init(gpioe_, 7, GPIO_INPUT | GPIO_PULL_UP);

    // --- Rotary Encoder Pushbuttons ---
    bottom_rotary_btn.init(gpiob_, 12, GPIO_INPUT | GPIO_PULL_UP);
    top_rotary_btn.init(gpioc_, 10, GPIO_INPUT | GPIO_PULL_UP);

    // --- Rotary Encoder Channels (Read as Digital Inputs) ---
    bottom_enc_a.init(gpioa_, 1, GPIO_INPUT | GPIO_PULL_UP);
    bottom_enc_b.init(gpioa_, 2, GPIO_INPUT | GPIO_PULL_UP);
    top_enc_a.init(gpioa_, 5, GPIO_INPUT | GPIO_PULL_UP);
    top_enc_b.init(gpioa_, 4, GPIO_INPUT | GPIO_PULL_UP);

    // --- Multiposition Switch (BCD Bits) ---
    multi_bit1.init(gpioc_, 5, GPIO_INPUT | GPIO_PULL_UP);
    multi_bit2.init(gpiob_, 1, GPIO_INPUT | GPIO_PULL_UP);
    multi_bit4.init(gpiob_, 2, GPIO_INPUT | GPIO_PULL_UP);
    multi_bit8.init(gpiob_, 0, GPIO_INPUT | GPIO_PULL_UP);

    // --- Status LEDs ---
    mcu_err_led.init(gpiob_, 13, GPIO_OUTPUT_INACTIVE);
    mcu_stat_led.init(gpiob_, 14, GPIO_OUTPUT_INACTIVE);

    LOG_INF("All Wheel GPIOs (16 inputs, 2 outputs) initialized");
    return 0;
}

int Hardware::initializeCAN()
{
    // Get FDCAN1 device
    can1_dev = DEVICE_DT_GET(DT_NODELABEL(fdcan1));

    if (!can1_dev)
    {
        LOG_ERR("Failed to get FDCAN1 device");
        return -1;
    }

    // Initialize CAN1 (1 Mbps)
    if (can1.init(can1_dev, 1000000, 875) != 0)
    {
        LOG_ERR("Failed to init CAN1");
        return -10;
    }

    if (can1.start() != 0)
    {
        LOG_ERR("Failed to start CAN1");
        return -11;
    }

    LOG_INF("FDCAN1 initialized and started at 1Mbps");
    return 0;
}