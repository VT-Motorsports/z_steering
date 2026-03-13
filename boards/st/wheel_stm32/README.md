# Custom Steering Wheel Controller — `wheel_stm32`

## Overview

The **Custom Steering Wheel Controller** is a board built around the **STM32H753** microcontroller. It is designed to act as the primary driver interface for the Virginia Tech Motorsports electric powertrain, gathering driver inputs and broadcasting them to the vehicle's control network.

The board relies entirely on digital GPIO polling (configured with internal pull-ups) to read a matrix of momentary pushbuttons, rotary encoders, and a 4-bit BCD multiposition switch. All vehicle state requests are packaged and transmitted via a robust CAN bus interface.

---

## SoC / Hardware

| Property        | Value                                 |
|-----------------|---------------------------------------|
| SoC             | STM32H753xx                           |
| Core            | ARM Cortex-M7 @ up to 480 MHz         |
| Flash           | 2 MB                                  |
| RAM             | 1 MB (including TCM)                  |
| CAN             | FDCAN1                                |
| Debug           | SWD + SWO                             |

---

## Supported Features

| Feature               | Config          | Notes                                     |
|-----------------------|-----------------|-------------------------------------------|
| CAN Bus (FDCAN)       | `fdcan1`        | Main vehicle bus communication            |
| Digital Inputs        | `gpio`          | Pushbuttons, Encoders, BCD (Internal Pull-ups) |
| Status LEDs           | `gpio`          | MCU Error and MCU Status                  |

---

## Pin Mapping

### Debug & Boot

| Pin   | Signal  | Function          |
|-------|---------|-------------------|
| NRST  | NRST    | Reset             |
| BOOT0 | BOOT0   | Boot mode select  |
| PA13  | SWDIO   | SWD Data          |
| PA14  | SWCLK   | SWD Clock         |
| PB3   | SWO     | Serial Wire Output|

### CAN Bus

| Pin        | Signal       | Peripheral     |
|------------|--------------|----------------|
| PD0 / PB9* | CAN Transmit | FDCAN1_TX      |
| PD1 / PB8* | CAN Receive  | FDCAN1_RX      |

*\*Check specific PCB schematic for final routed pin.*

### Driver Pushbuttons & Toggles (Digital Input)

| Pin  | Signal            | Description                        |
|------|-------------------|------------------------------------|
| PE6  | `btn_1`           | Momentary Pushbutton 1             |
| PE3  | `btn_2`           | Momentary Pushbutton 2             |
| PB15 | `btn_4`           | Momentary Pushbutton 4             |
| PE4  | `btn_5`           | Momentary Pushbutton 5             |
| PD6  | `btn_6`           | Momentary Pushbutton 6             |
| PE7  | `toggle_up`       | Flick-up Toggle Switch             |

### Rotary Encoders (Digital Input)

| Pin  | Signal              | Description                      |
|------|---------------------|----------------------------------|
| PB12 | `bottom_rotary_btn` | Bottom Encoder Push-Click        |
| PC10 | `top_rotary_btn`    | Top Encoder Push-Click           |
| PA1  | `bottom_enc_a`      | Bottom Encoder Quadrature A      |
| PA2  | `bottom_enc_b`      | Bottom Encoder Quadrature B      |
| PA5  | `top_enc_a`         | Top Encoder Quadrature A         |
| PA4  | `top_enc_b`         | Top Encoder Quadrature B         |

### Multiposition Switch (4-Bit BCD Input)

| Pin  | Signal         | Description                        |
|------|----------------|------------------------------------|
| PC5  | `multi_bit1`   | BCD Bit 0 (Value 1)                |
| PB1  | `multi_bit2`   | BCD Bit 1 (Value 2)                |
| PB2  | `multi_bit4`   | BCD Bit 2 (Value 4)                |
| PB0  | `multi_bit8`   | BCD Bit 3 (Value 8)                |

### Status LEDs (Digital Output)

| Pin  | Signal           | Colour / Function                  |
|------|------------------|------------------------------------|
| PB13 | `mcu_err_led`    | Red / Fault Indicator              |
| PB14 | `mcu_stat_led`   | Yellow / MCU Status & Heartbeat    |

---

## Board Directory Structure

When integrating this board into a Zephyr workspace, the expected directory layout is:

```text
boards/arm/wheel_stm32/
├── board.yml
├── Kconfig.board
├── Kconfig.defconfig
├── wheel_stm32.dts
├── wheel_stm32_defconfig
└── wheel_stm32-pinctrl.dtsi

---
## Building

```bash
west build -b nucleo_h753zi -p always
```
---
## Flashing / Debugging

The board exposes a standard **SWD** interface (PA13/PA14) with an additional **SWO** trace pin (PB3). Any ST-Link v2/v3 or J-Link probe may be used.

```bash
west flash
west debug
```

---

## References

- [STM32H753VIT6 Product Page](https://www.st.com/en/microcontrollers-microprocessors/stm32h753vi.html)
- [STM32H7 Reference Manual (RM0433)](https://www.st.com/resource/en/reference_manual/rm0433-stm32h742-stm32h743753-and-stm32h750-value-line-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
- [Zephyr Board Porting Guide](https://docs.zephyrproject.org/latest/hardware/porting/board_porting.html)
- [STM32H7 Zephyr SoC Support](https://docs.zephyrproject.org/latest/boards/st/)