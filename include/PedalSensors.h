#pragma once

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/can.h>
#include <zephyr/kernel.h>
#include <zephyr/logging\log.h>
#include <zephyr/types.h>

struct PedalSensor {
  const int inputPin;
  const int zeroPos;
  const int fullPos;
  const int onLedPin;
  const int ocLedPin;
  const int scLedPin;

  float pedalPercentage();
  bool checkOpenCircuit();
  bool checkShortCircuit();
  bool checkRangeFaults();
};

bool pedalAgreementFault(PedalSensor p1, PedalSensor p2);
bool brakeFault(PedalSensor p1, PedalSensor p2);
bool checkAllFaults(PedalSensor p1, PedalSensor p2)
