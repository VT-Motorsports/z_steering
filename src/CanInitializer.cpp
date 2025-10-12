#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/can.h>
#include <zephyr/kernel.h>
#include <zephyr/logging\log.h>
#include <zephyr/types.h>

// #define CANINITIALIZER
/*
    CAN INITIALIZATION SEQUENCE ::::
*/

#define CANINITIALIZER

// USER DEF CONFIG:
#ifdef CANINITIALIZER
// BAUD IN BITS PER SECOND
#define CAN_1_BAUD 1000000

// VAL/10 is percent of bit time where sampling is conducted
#define CAN_1_SAMPLE_POINT 875

// MESSAGE IDs USED IN THIS SECTION:
#define CAN_1_STATUS_MSG_ID 0x090

LOG_MODULE_REGISTER(canInitializer, LOG_LEVEL_INF);

inline void can_controller_state_print(const int8_t state) {
  LOG_INF("OP STATUS CODE: %i", state);
}

uint8_t canInit() {
  const struct device *CanController1 = DEVICE_DT_GET(DT_NODELABEL(fdcan1));
  if (device_is_ready(CanController1)) {
    LOG_INF("Can Controller 1 Has been Initialized");
  }

  struct can_timing *CanController1Timing;
  int8_t CanController1State;
  struct can_bus_err_cnt CanController1ErrorCount;

  struct can_frame status_frame = {
      CAN_1_STATUS_MSG_ID, can_bytes_to_dlc(8), 0, {0}};

  LOG_INF("Calculating Timing for CAN 1: ");
  CanController1State = can_calc_timing(CanController1, CanController1Timing,
                                        CAN_1_BAUD, CAN_1_SAMPLE_POINT);
  can_controller_state_print(CanController1State);

  LOG_INF("Stopping CAN 1 to set timing: ");
  CanController1State = can_stop(CanController1);
  can_controller_state_print(CanController1State);

  LOG_INF("Setting CAN 1 Timing: ");
  can_set_timing(CanController1, CanController1Timing);
  can_controller_state_print(CanController1State);

  LOG_INF("Restarting CAN 1 :");
  can_start(CanController1);
  can_controller_state_print(CanController1State);

  return 0;
}
#endif