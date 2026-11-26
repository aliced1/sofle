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
//   _FN    F-keys, arrows, tab switching, LED controls

#include QMK_KEYBOARD_H

enum layers { _BASE, _LEFT, _SYM, _FN };

// Text shortcuts (Symbols layer, bottom letter row): each types a phrase.
enum custom_keycodes {
    RS_SHORT = SAFE_RANGE, // types "R&S"
    RS_FULL,               // types "Rohde & Schwarz GmbH"
};

// Short name so it fits a 12-character column in the grid below.
#define SYM_BSPC LT(_SYM, KC_BSPC) // tap = Backspace, hold = symbols layer

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

// Grid: every key gets a 12-character column, labelled in the comment above each
// layer. To change a key, overtype it inside its column; nothing else moves.
//   L0-L5 / R0-R5  left / right half, left to right
//   inner          the two keys between the halves (bottom letter row only)
//   last row       five left thumb keys, then five right thumb keys
// Rows 1-3 have no inner keys, so those columns are blank there.
//
// Button box = left half, row 4: L0 = left button (F6), L1 = middle (F7),
// L2 = right (B1), L3 = not fitted (B3).
// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // Base: right-hand keys, 6-0 on top. Left half is the button box (bottom letter row).
    [_BASE] = LAYOUT(
        // L0       L1          L2          L3          L4          L5          inner       inner       R0          R1          R2          R3          R4          R5
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,                              KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_ESC,
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,                              KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       KC_DEL,
        KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,                              KC_H,       KC_J,       KC_K,       KC_L,       KC_SCLN,    OS_RSFT,
        OSL(_LEFT), OSL(_SYM),  OSL(_FN),   KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_N,       KC_M,       KC_COMM,    KC_DOT,     KC_SLSH,    OS_LCTL,
                                KC_NO,      KC_NO,      KC_NO,      KC_NO,      KC_NO,      SYM_BSPC,   KC_SPC,     OS_LALT,    KC_ENT,     KC_TAB
    ),

    // Left-hand keys (left half of Corne layer 0), shifted one column left so Q-T
    // sit under Y-P. The rightmost column falls through to the base layer.
    [_LEFT] = LAYOUT(
        // L0       L1          L2          L3          L4          L5          inner       inner       R0          R1          R2          R3          R4          R5
        _______,    _______,    _______,    _______,    _______,    _______,                            KC_1,       KC_2,       KC_3,       KC_4,       KC_5,       _______,
        _______,    _______,    _______,    _______,    _______,    _______,                            KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,       _______,
        _______,    _______,    _______,    _______,    _______,    _______,                            KC_A,       KC_S,       KC_D,       KC_F,       KC_G,       _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       _______,
                                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______
    ),

    // Symbols (right half of Corne layer 2).
    [_SYM] = LAYOUT(
        // L0       L1          L2          L3          L4          L5          inner       inner       R0          R1          R2          R3          R4          R5
        _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,    _______,    _______,    _______,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,                            KC_TILD,    KC_EXLM,    KC_BSLS,    KC_CIRC,    KC_NO,      _______,
        _______,    _______,    _______,    _______,    _______,    _______,                            KC_DLR,     KC_QUOT,    KC_PIPE,    KC_PERC,    KC_GRV,     _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RS_SHORT,   KC_DQT,     KC_AMPR,    RS_FULL,    KC_NO,      _______,
                                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______
    ),

    // Function: F1-F12, arrows (Up over Left/Down/Right), Ctrl+Tab / Ctrl+Shift+Tab
    // either side of Up (next / previous tab), and LED controls:
    //   RM_NEXT = next LED pattern, RM_TOGG = LEDs on/off, RM_VALU / RM_VALD = brighter / dimmer
    [_FN] = LAYOUT(
        // L0       L1          L2          L3          L4          L5          inner       inner       R0          R1          R2          R3          R4          R5
        _______,    _______,    _______,    _______,    _______,    _______,                            KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,      KC_F6,
        _______,    _______,    _______,    _______,    _______,    _______,                            KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,     KC_F12,
        _______,    _______,    _______,    _______,    _______,    _______,                            RM_NEXT,    C(KC_TAB),  KC_UP,      RCS(KC_TAB),RM_VALU,    _______,
        _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    RM_TOGG,    KC_LEFT,    KC_DOWN,    KC_RGHT,    RM_VALD,    _______,
                                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______
    ),
};
// clang-format on

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;
    switch (keycode) {
        case RS_SHORT:
            SEND_STRING("R&S");
            return false;
        case RS_FULL:
            SEND_STRING("Rohde & Schwarz GmbH");
            return false;
    }
    return true;
}

// QMK only applies RGB_MATRIX_MAXIMUM_BRIGHTNESS when brightness is changed, not to
// a value already saved on the keyboard. Clamp it at startup so a brightness saved
// by older firmware (up to 255) can't come back and brown out the board.
void keyboard_post_init_user(void) {
    if (rgb_matrix_get_val() > RGB_MATRIX_MAXIMUM_BRIGHTNESS) {
        rgb_matrix_sethsv(rgb_matrix_get_hue(), rgb_matrix_get_sat(), RGB_MATRIX_MAXIMUM_BRIGHTNESS);
    }
}

// Touchpad scroll speed. The TPS43 reports raw finger movement as scroll, so
// this divides it down: TOUCHPAD_SCROLL_DIVISOR in config.h (1 = unchanged,
// 2 = half speed, ...). Leftovers carry over, so slow scrolls still register.
report_mouse_t pointing_device_task_user(report_mouse_t report) {
    static int16_t carry_h = 0, carry_v = 0;
    carry_h += report.h;
    carry_v += report.v;
    report.h = carry_h / TOUCHPAD_SCROLL_DIVISOR;
    report.v = carry_v / TOUCHPAD_SCROLL_DIVISOR;
    carry_h -= report.h * TOUCHPAD_SCROLL_DIVISOR;
    carry_v -= report.v * TOUCHPAD_SCROLL_DIVISOR;
    return report;
}
