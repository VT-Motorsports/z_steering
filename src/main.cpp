/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef __cplusplus
#error "__cplusplus not defined! Build system is compiling as C!"
#endif

#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <sys/_intsup.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/can.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/types.h>
#ifdef __cplusplus
}
#endif

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main);

#include "CanInitializer.h"
// #include "PedalSensors.h"

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS 1000

#define DEBUGGING 0

// Placeholder values while we do not have real numbers (most likely same as 24
// // car)
// const int ocThreshold = 1; // Open Circuit threshold
// const int scThreshold = 2; // Short Circuit threshold

// // Pin Configuration NOT REAL VALUES
// const int brake_switch_pin = 14;
// const int brake_out_pin = 2;
// const int error_out_pin = 9;
// const int rtds_horn_pin = 21;
// const int custom_pedal_pin = A14;

// // Pedal sensor1 WRITE DOWN RIGHT OR LEFT WHEN WE FIGURE IT OUT also update
// // values
// const PedalSensor p1 = {0, 0, 0, 0, 0, 0};

// // Pedal sensor2 WRITE DOWN RIGHT OR LEFT WHEN WE FIGURE IT OUT also update
// // values
// const PedalSensor p2 = {0, 0, 0, 0, 0, 0};

/* The devicetree node identifier for the "led0" alias. */

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */

int main(void) {

    LOG_INF("ENTERED MAIN"); 
    can_init();
}
