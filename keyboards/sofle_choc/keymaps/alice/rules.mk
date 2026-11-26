# Both halves run a 0xCB Helios (RP2040) in the Pro Micro footprint.
CONVERT_TO = helios

# Azoteq TPS43 touchpad on the right half, on the OLED header's I2C pins
# (D1/D0 = GP2/GP3; the Helios converter already routes I2C1 there).
POINTING_DEVICE_ENABLE = yes
POINTING_DEVICE_DRIVER = azoteq_iqs5xx

# Carried over from the crkbd aliced1 keymap.
COMBO_ENABLE = yes
DYNAMIC_MACRO_ENABLE = yes
REPEAT_KEY_ENABLE = yes

# No OLED: the touchpad uses the OLED header's I2C pins.
OLED_ENABLE = no

# Right-half volume encoder (click = mute is a matrix key, [9,5]).
ENCODER_MAP_ENABLE = yes
CAPS_WORD_ENABLE = yes
