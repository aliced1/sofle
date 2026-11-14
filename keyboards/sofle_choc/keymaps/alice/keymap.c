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
//   _FN    function/media

#include QMK_KEYBOARD_H

enum layers { _BASE, _LEFT, _SYM, _FN };

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
    [_FN] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______, QK_BOOT,
        _______, _______, _______, _______, _______, _______,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
        _______, _______, _______, _______, _______, _______,            KC_BRIU, LCS(KC_V), LSFT(KC_INS), DM_REC2, DM_RSTP, _______,
        _______, _______, _______, _______, _______, _______, _______,   _______, KC_BRID, LSG(KC_S), KC_NO, DM_PLY2, KC_NO,   _______,
                 _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______
    ),
};
