// One-handed Sofle Choc: a right half with a touchpad, plus a left-side button box.
//
// Build: qmk compile -kb sofle_choc -km alice
// Flash the same .uf2 to both halves. USB always goes into the RIGHT half.
//
// The left "half" is the button box: up to four switches on matrix row B4
// (row 3). Three are fitted, on column pins F6 / F7 / B1 (left / middle / right);
// B3 is unused.
// Each button is OSL(): tap = the next keypress uses that layer; hold = the
// layer stays active while held.
//
// Layers on the right half (from the crkbd aliced1 keymap, minus its numpad
// layer -- the Sofle's extra top row holds the numbers instead):
//   _BASE  right-hand keys, 6-0 on top
//   _LEFT  left-hand keys in their normal order, 1-5 on top
//   _SYM   symbols
//   _FN    function/media, plus the touchpad scroll-speed keys
//
// Scroll speed: SCRL_SLOW / SCRL_FAST step the touchpad's two-finger scroll
// speed down / up. The setting is saved on the keyboard, so it survives
// unplugging. Pointer movement speed is not affected.

#include QMK_KEYBOARD_H

enum layers { _BASE, _LEFT, _SYM, _FN };

// User keycodes (QK_USER_n) rather than SAFE_RANGE, so a Vial/VIA build can
// show them as named custom keys later.
enum custom_keycodes {
    SCRL_SLOW = QK_USER_0,
    SCRL_FAST = QK_USER_1,
};

// Combos from the Corne keymap. The left-hand ones fire on the _LEFT layer.
const uint16_t PROGMEM y_u_rprn[] = {KC_Y, KC_U, COMBO_END};
const uint16_t PROGMEM h_j_rbrc[] = {KC_H, KC_J, COMBO_END};
const uint16_t PROGMEM n_m_rcbr[] = {KC_N, KC_M, COMBO_END};
const uint16_t PROGMEM r_t_lprn[] = {KC_R, KC_T, COMBO_END};
const uint16_t PROGMEM f_g_lbrc[] = {KC_F, KC_G, COMBO_END};
const uint16_t PROGMEM v_b_lcbr[] = {KC_V, KC_B, COMBO_END};
const uint16_t PROGMEM j_k_esc[]  = {KC_J, KC_K, COMBO_END};

combo_t key_combos[] = {
    COMBO(y_u_rprn, KC_RPRN),
    COMBO(h_j_rbrc, KC_RBRC),
    COMBO(n_m_rcbr, KC_RCBR),
    COMBO(r_t_lprn, KC_LPRN),
    COMBO(f_g_lbrc, KC_LBRC),
    COMBO(v_b_lcbr, KC_LCBR),
    COMBO(j_k_esc, KC_ESC),
};

// Layout order per row: six left-half keys, then six right-half keys
// (left to right). The bottom letter row also has the two inner keys between
// the halves, and the last row is five left thumb keys then five right ones.
//
// Button box, bottom-left row, physical left to right:
//   F6 = left button, F7 = middle button, B1 = right button, B3 = not fitted
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        // left half: button box only                                                  right half
        KC_NO,   KC_NO,     KC_NO,     KC_NO,      KC_NO,   KC_NO,              KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_GRV,
        KC_NO,   KC_NO,     KC_NO,     KC_NO,      KC_NO,   KC_NO,              KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
        KC_NO,   KC_NO,     KC_NO,     KC_NO,      KC_NO,   KC_NO,              KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_RSFT,
        OSL(_LEFT), OSL(_SYM), OSL(_FN), KC_NO,    KC_NO,   KC_NO,   KC_NO,     KC_NO,   KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, OS_LCTL,
                 KC_NO,     KC_NO,     KC_NO,      KC_NO,   KC_NO,              LT(_SYM, KC_BSPC), KC_SPC, OS_LALT, KC_ENT, KC_TAB  // Enter/Tab: placeholders
    ),

    // Left-hand keys (left half of Corne layer 0), same order, on the right half.
    [_LEFT] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,
        _______, _______, _______, _______, _______, _______,            KC_ESC,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,
        _______, _______, _______, _______, _______, _______,            OS_LSFT, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,
        _______, _______, _______, _______, _______, _______, _______,   _______, OS_LCTL, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,
                 _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______
    ),

    // Symbols (right half of Corne layer 2)
    [_SYM] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______, _______,
        _______, _______, _______, _______, _______, _______,            KC_TILD, KC_EXLM, KC_BSLS, KC_CIRC, KC_NO,   _______,
        _______, _______, _______, _______, _______, _______,            KC_DLR,  KC_QUOT, KC_PIPE, KC_PERC, KC_NO,   _______,
        _______, _______, _______, _______, _______, _______, _______,   _______, KC_NO,   KC_DQT,  KC_AMPR, KC_NO,   KC_NO,   _______,
                 _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______
    ),

    // Function/Media (right half of Corne layer 3). QK_BOOT top-right for reflashing.
    // Scroll speed: slower on the comma key, faster on the slash key.
    [_FN] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, _______, _______, _______, _______, _______,            KC_BRIU, LCS(KC_V), LSFT(KC_INS), DM_REC2, DM_RSTP, _______,
        _______, _______, _______, _______, _______, _______, _______,   _______, KC_BRID, LSG(KC_S), SCRL_SLOW, DM_PLY2, SCRL_FAST, _______,
                 _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______
    ),
};

// ---------------------------------------------------------------------------
// Touchpad scroll speed
//
// The Azoteq driver reports two-finger scrolling as raw h/v deltas. Each
// report is scaled by a percentage from the table below; the leftover
// fraction is carried to the next report, so slow settings still scroll
// smoothly instead of dropping small movements.
// ---------------------------------------------------------------------------

static const uint16_t scroll_pct[] = {20, 30, 45, 65, 100, 150, 225, 340, 500};
#define SCROLL_LEVELS (sizeof(scroll_pct) / sizeof(scroll_pct[0]))
#define SCROLL_DEFAULT_LEVEL 4 // 100%: the driver's own speed
#define USER_CONFIG_MAGIC 0xA5  // marks the saved setting as ours

typedef union {
    uint32_t raw;
    struct {
        uint8_t scroll_level;
        uint8_t magic;
    };
} user_config_t;

static user_config_t user_config;

static void save_user_config(void) {
    eeconfig_update_user(user_config.raw);
}

static void reset_user_config(void) {
    user_config.raw          = 0;
    user_config.scroll_level = SCROLL_DEFAULT_LEVEL;
    user_config.magic        = USER_CONFIG_MAGIC;
    save_user_config();
}

// Runs when the keyboard's settings storage is wiped (EE_CLR, or first boot).
void eeconfig_init_user(void) {
    reset_user_config();
}

void keyboard_post_init_user(void) {
    user_config.raw = eeconfig_read_user();
    // Storage written by an older firmware, or out of range: start at 100%.
    if (user_config.magic != USER_CONFIG_MAGIC || user_config.scroll_level >= SCROLL_LEVELS) {
        reset_user_config();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) {
        return true;
    }
    switch (keycode) {
        case SCRL_SLOW:
            if (user_config.scroll_level > 0) {
                user_config.scroll_level--;
                save_user_config();
            }
            return false;
        case SCRL_FAST:
            if (user_config.scroll_level < SCROLL_LEVELS - 1) {
                user_config.scroll_level++;
                save_user_config();
            }
            return false;
    }
    return true;
}

// Scales one scroll axis, keeping the remainder in *acc (in 1/100ths).
static mouse_hv_report_t scale_scroll(mouse_hv_report_t value, int32_t *acc) {
    if (value == 0) {
        return 0;
    }
    // Drop the leftover when the direction reverses, so it doesn't lag.
    if ((value > 0 && *acc < 0) || (value < 0 && *acc > 0)) {
        *acc = 0;
    }
    *acc += (int32_t)value * scroll_pct[user_config.scroll_level];
    int32_t out = *acc / 100;
    *acc -= out * 100;
    if (out > 127) out = 127;
    if (out < -127) out = -127;
    return (mouse_hv_report_t)out;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    static int32_t acc_h = 0, acc_v = 0;
    mouse_report.h = scale_scroll(mouse_report.h, &acc_h);
    mouse_report.v = scale_scroll(mouse_report.v, &acc_v);
    return mouse_report;
}
