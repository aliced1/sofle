// One-handed Sofle Choc: a right half with a touchpad and volume encoder, plus
// a left-side button box worked by the left fist.
//
// Build: qmk compile -kb sofle_choc -km alice
// Flash the same .uf2 to both halves. USB always goes into the RIGHT half.
//
// The left "half" is the button box: up to four switches on matrix row B4
// (row 3). Three are fitted, on column pins F6 / F7 / B1 (left / middle / right);
// B3 is unused. Each button is OSL(): tap = the next keypress uses that layer;
// hold = the layer stays active while held.
//
// Layers:
//   _BASE    right-hand keys, 6-0 on top
//   _MIRROR  half-QWERTY: the left-hand keys mirrored onto the right hand
//            (T over Y, G over H, B over N, ... Q over P, A over ;, Z over /).
//            Hold Space, or tap the left button, to use it.
//   _SYM     symbols
//   _NAV     arrows, tab switching, copy/paste, Rohde & Schwarz macros
//   _FN      F-keys, media, bootloader -- hold Enter
//
// The outer column is the same on every layer: Del / Shift / Ctrl.
//
// Thumb row (right half, inner -> outer), so the right hand alone can reach
// every layer. Keys are ordered by how often they're used, placed by how easy
// each position is to hit (ease rank 3,1,2,4,5 -- the thumb rests on the
// second key, not the big inner one):
//   ease:  3        1        2        4        5
//   tap:   Tab      Space    Bksp     Enter    one-shot MIRROR (next key only)
//   hold:  NAV      MIRROR   SYM      FN       Alt
//
// J+K = Repeat Key (repeats the last key, e.g. a run of Enters).
//
// Touchpad scroll speed uses the level saved on the keyboard (see the end of
// this file). Pointer movement speed is not affected.

#include QMK_KEYBOARD_H

enum layers { _BASE, _MIRROR, _SYM, _NAV, _FN };

enum custom_keycodes {
    RS_FULL = SAFE_RANGE, // "Rohde & Schwarz GmbH & Co. KG"
    RS_SHORT,             // "R&S"
};

// Thumb keys
#define SPC_MIR LT(_MIRROR, KC_SPC)
#define BSP_SYM LT(_SYM, KC_BSPC)
#define TAB_NAV LT(_NAV, KC_TAB)
#define ENT_FN  LT(_FN, KC_ENT)
// Hold = Alt. The tap keycode (F24) is never sent: process_record_user turns
// a tap into a one-shot MIRROR.
#define MIR_ALT LALT_T(KC_F24)

// Shortcuts used on the NAV layer
#define TAB_PRV C(S(KC_TAB))
#define TAB_NXT C(KC_TAB)
#define UNDO    C(KC_Z)
#define CUT     C(KC_X)
#define COPY    C(KC_C)
#define PASTE   C(KC_V)
#define REDO    C(KC_Y)

// J+K = Repeat Key. J+K rather than U+I: "ui" turns up constantly in contract
// language (require, acquire, suit, equity, liquidated), and a fast one-handed
// roll of U then I could fire the combo. "jk" essentially never occurs in
// English. Combos match keycodes, so this only fires on _BASE (on _MIRROR
// those keys are F and D).
const uint16_t PROGMEM j_k_rep[] = {KC_J, KC_K, COMBO_END};

combo_t key_combos[] = {
    COMBO(j_k_rep, QK_REP),
};

// Layout order per row: six left-half keys, then six right-half keys
// (left to right). The bottom letter row also has the two encoder push-buttons
// between the halves ([4,5] left -- not fitted, [9,5] right), and the last row
// is five left thumb keys then five right ones.
//
// Button box, bottom-left row, physical left to right:
//   F6 = left button, F7 = middle button, B1 = right button, B3 = not fitted
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        // left half: button box only                                                       right half
        KC_NO,        KC_NO,       KC_NO,       KC_NO, KC_NO, KC_NO,                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_ESC,
        KC_NO,        KC_NO,       KC_NO,       KC_NO, KC_NO, KC_NO,                          KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_DEL,
        KC_NO,        KC_NO,       KC_NO,       KC_NO, KC_NO, KC_NO,                          KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, OS_RSFT,
        OSL(_MIRROR), OSL(_SYM),   OSL(_NAV),   KC_NO, KC_NO, KC_NO, KC_NO,         KC_MUTE,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, OS_LCTL,
                      KC_NO,       KC_NO,       KC_NO, KC_NO, KC_NO,                          TAB_NAV, SPC_MIR, BSP_SYM, ENT_FN,  MIR_ALT
    ),

    // Half-QWERTY: the left half of a QWERTY keyboard, mirrored.
    [_MIRROR] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            KC_5,    KC_4,    KC_3,    KC_2,    KC_1,    _______,
        _______, _______, _______, _______, _______, _______,            KC_T,    KC_R,    KC_E,    KC_W,    KC_Q,    _______,
        _______, _______, _______, _______, _______, _______,            KC_G,    KC_F,    KC_D,    KC_S,    KC_A,    _______,
        _______, _______, _______, _______, _______, _______, _______,   _______, KC_B,    KC_V,    KC_C,    KC_X,    KC_Z,    _______,
                 _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______
    ),

    // Symbols. Pairs not listed are Shift + a base key, e.g. Shift+[ = {,
    // Shift+= = +, Shift+\ = |, Shift+- = _, Shift+; = :, Shift+/ = ?.
    [_SYM] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC, _______,
        _______, _______, _______, _______, _______, _______,            KC_TILD, KC_QUOT, KC_DQUO, KC_GRV,  KC_CIRC, _______,
        _______, _______, _______, _______, _______, _______,            KC_MINS, KC_AMPR, KC_SLSH, KC_LPRN, KC_RPRN, _______,
        _______, _______, _______, _______, _______, _______, _______,   _______, KC_LBRC, KC_RBRC, KC_BSLS, KC_EQL,  KC_ASTR, _______,
                 _______, _______, _______, _______, _______,            _______, _______, _______, _______, _______
    ),

    // Navigation and editing. Arrows are an inverted T on U / H J K.
    [_NAV] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            RS_SHORT, RS_FULL, CW_TOGG, QK_REP,  KC_NO,   _______,
        _______, _______, _______, _______, _______, _______,            TAB_PRV,  KC_UP,   TAB_NXT, KC_PGUP, KC_HOME, _______,
        _______, _______, _______, _______, _______, _______,            KC_LEFT,  KC_DOWN, KC_RGHT, KC_PGDN, KC_END,  _______,
        _______, _______, _______, _______, _______, _______, _______,   _______,  UNDO,    CUT,     COPY,    PASTE,   REDO,    _______,
                 _______, _______, _______, _______, _______,            _______,  _______, _______, _______, _______
    ),

    // F-keys and odds and ends. QK_BOOT top-right for reflashing.
    [_FN] = LAYOUT(
        _______, _______, _______, _______, _______, _______,            KC_F1,     KC_F2,     KC_F3,   KC_F4,   KC_F5,        QK_BOOT,
        _______, _______, _______, _______, _______, _______,            KC_F6,     KC_F7,     KC_F8,   KC_F9,   KC_F10,       _______,
        _______, _______, _______, _______, _______, _______,            KC_F11,    KC_F12,    KC_BRIU, DM_REC2, DM_RSTP,      _______,
        _______, _______, _______, _______, _______, _______, _______,   _______, LSG(KC_S), LCS(KC_V), KC_BRID, DM_PLY2, LSFT(KC_INS), _______,
                 _______, _______, _______, _______, _______,            _______,   _______,   _______, _______, _______
    ),
};

// Left encoder isn't fitted; right encoder is volume on every layer.
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [_BASE]   = { ENCODER_CCW_CW(KC_NO, KC_NO), ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_MIRROR] = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [_SYM]    = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [_NAV]    = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
    [_FN]     = { ENCODER_CCW_CW(_______, _______), ENCODER_CCW_CW(_______, _______) },
};

// Half-QWERTY needs Space to become the mirror layer even right after a
// tapped space ("the cat": tap Space, then hold Space + , for C). The default
// quick-tap behaviour would turn that second press into a held, repeating
// space. Backspace keeps quick-tap, so tap-then-hold still repeats it.
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SPC_MIR:
            return 0;
        default:
            return QUICK_TAP_TERM;
    }
}

// Don't let a one-shot-MIRROR tap become the "last key" -- Repeat would send F24.
bool remember_last_key_user(uint16_t keycode, keyrecord_t *record, uint8_t *remembered_mods) {
    return keycode != MIR_ALT;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MIR_ALT:
            if (record->tap.count) { // tapped: behave like OSL(_MIRROR)
                if (record->event.pressed) {
                    set_oneshot_layer(_MIRROR, ONESHOT_START);
                } else {
                    clear_oneshot_layer_state(ONESHOT_PRESSED);
                }
                return false;
            }
            return true; // held: Alt
        case RS_FULL:
            if (record->event.pressed) {
                clear_oneshot_mods();
                SEND_STRING("Rohde & Schwarz GmbH & Co. KG");
            }
            return false;
        case RS_SHORT:
            if (record->event.pressed) {
                clear_oneshot_mods();
                SEND_STRING("R&S");
            }
            return false;
    }
    return true;
}

// ---------------------------------------------------------------------------
// Touchpad scroll speed
//
// The Azoteq driver reports two-finger scrolling as raw h/v deltas. Each
// report is scaled by a percentage from the table below; the leftover
// fraction is carried to the next report, so slow settings still scroll
// smoothly instead of dropping small movements.
//
// The level is read from the keyboard's settings storage, where an earlier
// firmware (with scroll-speed keys) saved it. There are no keys to change it
// now; the saved level is kept as-is.
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

static void reset_user_config(void) {
    user_config.raw          = 0;
    user_config.scroll_level = SCROLL_DEFAULT_LEVEL;
    user_config.magic        = USER_CONFIG_MAGIC;
    eeconfig_update_user(user_config.raw);
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
