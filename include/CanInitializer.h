#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/can.h>
#include <zephyr/kernel.h>
#include <zephyr/logging\log.h>
#include <zephyr/types.h>

#pragma once

/* can_init ENCAPSULATES ALL CAN INITIALIZATION AND STARTUP CONFIGURATION
 * BEHAVIOR */
uint8_t can_init();
