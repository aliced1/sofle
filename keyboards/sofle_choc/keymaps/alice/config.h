#pragma once

// USB always goes into the right half (it has the touchpad), so the half
// with USB is the right half. Both halves can run the same firmware.
#define MASTER_RIGHT

// Double-tap the reset button to enter the RP2040 bootloader.
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

// Touchpad: Azoteq TPS43.
#define AZOTEQ_IQS5XX_TPS43
#define AZOTEQ_IQS5XX_TAP_ENABLE true            // one-finger tap = left click
#define AZOTEQ_IQS5XX_TWO_FINGER_TAP_ENABLE true // two-finger tap = right click
#define AZOTEQ_IQS5XX_PRESS_AND_HOLD_ENABLE true // tap-and-hold = drag
#define AZOTEQ_IQS5XX_SCROLL_ENABLE true         // two-finger drag = scroll
// The pad is mounted upside down: without this, both axes were reversed.
#define AZOTEQ_IQS5XX_ROTATION_180

#define DYNAMIC_MACRO_NO_NESTING
