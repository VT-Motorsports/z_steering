#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include "hardware.h"
#include "system.h"
#include "steering_state.h"

LOG_MODULE_REGISTER(main);

int main(void)
{
    LOG_INF("***STEERING WHEEL ENTERED MAIN***");

    // Static instances to ensure they live for the duration of the program
    static WheelState wheel;
    static Hardware   hardware(&wheel);
    static System      system;

    LOG_INF("=== Wheel Node Starting ===");

    // Initialize RTOS resources (Heap, etc.)
    if (system.init() != 0)
    {
        LOG_ERR("System init failed!");
        return -1;
    }

    // Initialize FDCAN1 and all 16 Digital Inputs
    if (hardware.init() != 0)
    {
        LOG_ERR("Hardware init failed!");
        return -2;
    }

    /*Start the Steering Input Task here.
     * This will replace the old 'apps_task' and will poll the buttons,
     * encoders, and BCD switch.*/
    start_steering_input_task(&wheel, &hardware);

    // Start the system diagnostics task (Uptime, CPU Load, CAN stats)
    start_diagnostics_task(&system, &hardware, &wheel);

    LOG_INF("=== Wheel Node Ready ===");

    // Main thread enters a forever sleep as periodic tasks handle the work
    while (1)
    {
        k_sleep(K_FOREVER);
    }
}