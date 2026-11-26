#pragma once

// USB always goes into the right half (it has the touchpad), so the half
// with USB is the right half. Both halves can run the same firmware.
#define MASTER_RIGHT

// Double-tap the reset button to enter the RP2040 bootloader.
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 2000U

// Touchpad: Azoteq TPS43.
#define AZOTEQ_IQS5XX_TPS43
#define AZOTEQ_IQS5XX_TAP_ENABLE true            // one-finger tap = left click
#define AZOTEQ_IQS5XX_TWO_FINGER_TAP_ENABLE true // two-finger tap = right click
#define AZOTEQ_IQS5XX_PRESS_AND_HOLD_ENABLE true // tap-and-hold = drag
#define AZOTEQ_IQS5XX_SCROLL_ENABLE true         // two-finger drag = scroll
// The pad is mounted upside down: without this, both axes were reversed.
#define AZOTEQ_IQS5XX_ROTATION_180

// Scroll speed: raise to scroll slower (whole numbers, 1 = the pad's raw speed).
// To scroll faster than 1, use Windows' "lines to scroll at a time" instead.
#define TOUCHPAD_SCROLL_DIVISOR 10

// LEDs: cap brightness (0-255). The board's own default keymap uses 100 and warns
// that higher "may cause the controller to crash": 29 LEDs at full brightness
// draw more than USB supplies, the voltage sags, and the Helios resets.
#define RGB_MATRIX_MAXIMUM_BRIGHTNESS 200 // 100 (board default) was too dim; 130 tested stable
#define RGB_MATRIX_DEFAULT_VAL 50

// Reactive LED patterns (light up as keys are pressed). They need keypress
// tracking. RM_NEXT cycles through these along with the board's 29 patterns;
// delete any line here to drop that pattern from the cycle.
#define RGB_MATRIX_KEYPRESSES
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_SIMPLE     // pressed key lights, then fades
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE            // same, with a colour shift
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE       // pressed key + a glow around it
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE  //   ...for several keys at once
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_CROSS      // lights its row and column
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTICROSS //   ...for several keys at once
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_NEXUS      // lines shoot out along row/column
#define ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTINEXUS //   ...for several keys at once
#define ENABLE_RGB_MATRIX_SPLASH                    // rainbow ripple out from the key
#define ENABLE_RGB_MATRIX_MULTISPLASH               //   ...for several keys at once
#define ENABLE_RGB_MATRIX_SOLID_SPLASH              // single-colour ripple
#define ENABLE_RGB_MATRIX_SOLID_MULTISPLASH         //   ...for several keys at once

#define DYNAMIC_MACRO_NO_NESTING

// Thumb keys are tap-hold. Permissive hold: if another key is pressed AND
// released while a thumb key is down, it counts as a hold even if it was
// quick -- needed for fast half-QWERTY (hold Space, tap a letter).
#define PERMISSIVE_HOLD
#define QUICK_TAP_TERM_PER_KEY
