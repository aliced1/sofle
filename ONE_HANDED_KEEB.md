# One-handed Sofle keyboard: project context

## Goal

Build a one-handed keyboard for the user's disabled sister. She types with her right hand. Her left hand has severe nerve damage, but she can press (and possibly hold) large buttons.

- **Right hand:** the right half of a **Sofle Choc** split keyboard (QMK board `sofle_choc`), with at least four layers.
- **Left hand:** no normal left half. Instead, a small "button box" with three large buttons that switch layers on the right half.
- An **Azoteq TPS43** touchpad (bought from HolyKeebs) is installed on the right half.

## Status (2026-11-12)

- Hardware is built. Firmware compiles and has been flashed to both halves: typing on the right half works, the split link works, and the touchpad works. A box button that does nothing at all is likely the box half missing this firmware (both halves must be flashed once).
- The touchpad works (so the OLED-header I2C wiring assumption holds), with the 180° rotation fix.
- **Touchpad repair (2026-11-24):** the touchpad stopped responding (an I2C scan found nothing at 0x74). Measured against a Helios GND, every touchpad pad, including its GND, read 3.3 V: the touchpad's ground was open, so the chip was unpowered. The cause was the ribbon cable between the pin-mount PCB and the touchpad PCB. The user replaced it with four hand-soldered wires (VCC, GND, SDA, SCL), and it works again. The QMK Azoteq driver uses no RDY/RST pins, so those four are all it needs. If it dies again, check that ground first.
- Not yet confirmed on hardware: the button-box layer keys, the touchpad gestures, the combos, and the rotary encoder.

## Why a button box instead of a USB macropad

A separate USB macropad can't change layers on another keyboard: each runs its own firmware, and the host doesn't relay keypresses between keyboards. Instead, the button box *is* the Sofle's left half as far as QMK is concerned: its own controller, connected to the right half over the normal TRRS split cable, with its buttons wired into left-half matrix positions. QMK merges both halves into one keyboard, so the buttons can be ordinary layer keys.

## Hardware

### Controllers

- **Both halves:** 0xCB Helios (RP2040, Pro Micro footprint). The button box's Helios sits on a prototyping board in a 3D-printed case.
- On the Sofle PCB the controller is mounted **face down**, which mirrors its pin columns. Always identify pins by their printed names, not left/right position.
- No OLED on the right half. The touchpad uses the OLED header's I2C pins (D1/D0 = GP2/GP3); confirmed working.

### Button box wiring

Wired to the Sofle left-half matrix. `sofle_choc` and `sofle/rev1` have **identical** matrix pins (rows C6, D7, E6, B4, B5; cols F6, F7, B1, B3, B2, B6), diode direction (COL2ROW), split pin (D2), and 60-key layout order, so the wiring is correct for `sofle_choc`.

| Helios pin | Connects to | Notes |
| --- | --- | --- |
| D2 | TRRS data | Split serial |
| VCC | TRRS VCC | 3.3V on the Helios |
| GND | TRRS GND | |
| B4 | Shared row wire for all buttons | Matrix row 3 |
| F6, F7, B1 | Left, middle, right button | Column side of each switch. **Confirmed by testing** (each position mapped to a digit). B3 is not fitted, despite the original plan listing B3/B1/F7/F6 as SW1–SW4. |
| RST and GND | Reset tactile switch | Outside the matrix, no diode |

- Each button: column pin → mechanical switch → 1N4148 diode → row B4. Diode stripe (cathode) faces the row. One shared row and separate columns, so any combination can be held with no ghosting.
- Matrix positions: left = [3,0] (F6), middle = [3,1] (F7), right = [3,2] (B1); [3,3] (B3) unused. Bottom letter row of the left half.
- **Mechanical:** the switches sit in hot-swap sockets held only by their pins, so presses flex the solder joints. The right button (B1) was intermittent, likely for this reason. The user plans to bed the switches in thermoplastic; a 3D-printed switch plate is the sturdier alternative. If a button is flaky, suspect joints/wires first, not firmware.
- Helios GPIO mapping, **verified** against QMK's converter table (`platforms/chibios/converters/promicro_to_rp2040_ce/_pin_defs.h`): D2 = GP1, B4 = GP8, B3 = GP20, B1 = GP22, F7 = GP26, F6 = GP27, D1 = GP2, D0 = GP3.

### TRRS jack, measured on the right half

From the contact nearest the jack's opening:

1. D2 (data)
2. GND
3. VCC
4. Tip: goes only to a pad labeled P1 near R1 on the right PCB, most likely the optional I2C split link. Unused in serial mode; left unconnected in the box.

Never plug or unplug the TRRS cable while USB is connected.

## Firmware

### Repo and approach

- Repo: `/home/duffa1/sofle`, a clone of the HolyKeebs QMK fork (remote `aliced1/sofle`, branch `hk-master`).
- **The HolyKeebs userspace is NOT used.** That repo's `CLAUDE.md` describes building holykeebs boards with `USER_NAME=holykeebs` and an external userspace overlay; ignore that for this keyboard. Mainline QMK's `azoteq_iqs5xx` driver handles the TPS43 directly (tap = left click, two-finger tap = right click, tap-and-hold = drag, two-finger drag = scroll), so no overlay or holykeebs board dir is needed.
- The user doesn't want "holykeebs" in their board or keymap paths unless it's unavoidable. Don't copy the `keyboards/holykeebs/corne` pattern.
- The firmware is a **keymap of the stock `sofle_choc` board**: `keyboards/sofle_choc/keymaps/alice/`. (It was briefly a `sofle/alice` board dir before we found the PCB is the Choc version. `sofle_choc` has its `keyboard.json` at the top level, so it can't have child board dirs; a keymap is the right shape.)

### Build

```
qmk compile -kb sofle_choc -km alice
```

Output: `/home/duffa1/sofle/sofle_choc_alice.uf2`. No `-e` flags are needed, because the keymap's `rules.mk` sets `CONVERT_TO = helios`.

### Flashing

- The keymap lives on the master, so a keymap change only needs flashing to the **right half**. The first flash, or anything that changes split/matrix behaviour, goes to both halves (same `.uf2`).
- To enter the bootloader: double-tap the reset button (`RP2040_BOOTLOADER_DOUBLE_TAP_RESET` is on), or hold the right half's top outer key (`` ` ``, matrix [5,0]) while plugging in. (`QK_BOOT` was removed from the keymap at the user's request.) Then copy the `.uf2` to the RPI-RP2 drive.
- **USB always goes into the right half.** `MASTER_RIGHT` is hard-coded, so the half with USB is treated as the right half.

### Build environment setup (already done on this machine; for reference)

The machine is WSL2 Ubuntu with **no sudo**, so everything was installed into the user's home:

1. **Git submodules** (ChibiOS, pico-sdk, etc.) were missing: `git submodule update --init --recursive`.
2. **QMK CLI + toolchain** via QMK's bootstrap, skipping steps that need root:
   ```
   SKIP_PACKAGE_MANAGER=1 SKIP_UDEV_RULES=1 SKIP_WINDOWS_DRIVERS=1 SKIP_CLEAN=1 sh util/env-bootstrap.sh --confirm
   ```
   This installs `uv` and `qmk` into `~/.local/bin`. It **failed to unpack the toolchain** because `zstd`/`zstdcat` isn't installed (and needs apt). Workaround:
   ```
   cd ~/.local/share/qmk
   ~/.local/bin/uv run --no-project --with zstandard python -c "
   import zstandard,tarfile
   with open('qmk_toolchains-gcc15.2.0-host_linuxX64.tar.zst','rb') as f:
       tarfile.open(fileobj=zstandard.ZstdDecompressor().stream_reader(f),mode='r|').extractall('.',filter='tar')"
   cp -a qmk_toolchains_linuxX64/. . && rm -rf qmk_toolchains_linuxX64 *.tar.zst
   ```
   The toolchain must sit directly in `~/.local/share/qmk` (`bin/arm-none-eabi-gcc`), not in a subfolder, or the build fails with `arm-none-eabi-gcc: not found`.
3. If `qmk` isn't on PATH in a new shell: `export PATH=$HOME/.local/bin:$PATH`.

### Keymap config (`keyboards/sofle_choc/keymaps/alice/`)

`rules.mk`:
- `CONVERT_TO = helios`: maps Pro Micro pin names to RP2040 and sets the vendor serial driver, I2C1 on D1/D0, and USB VBUS detect on GP19.
- `POINTING_DEVICE_ENABLE = yes`, `POINTING_DEVICE_DRIVER = azoteq_iqs5xx`.
- `OLED_ENABLE = no`: the board enables OLED by default, and setting `"oled": false` in a JSON override didn't take effect, so it's disabled here.
- `COMBO_ENABLE`, `DYNAMIC_MACRO_ENABLE`, `REPEAT_KEY_ENABLE` (carried over from the Corne keymap).

`config.h`:
- `MASTER_RIGHT`. (`split.main` in JSON is deprecated and didn't emit it.)
- `RP2040_BOOTLOADER_DOUBLE_TAP_RESET`.
- `AZOTEQ_IQS5XX_TPS43`, plus tap, two-finger tap, press-and-hold and scroll enabled.
- `AZOTEQ_IQS5XX_ROTATION_180`: the pad is mounted upside down. Without it, both axes were reversed (confirmed by the user on hardware).

### Keymap layout

Ported from the user's Corne keymap (`/mnt/c/Users/Alice/qmk_firmware/keyboards/crkbd/keymaps/aliced1`, built there with `qmk compile -e CONVERT_TO=blok -kb crkbd/r2g -km aliced1`). The Corne's numpad layer was dropped on purpose: the Sofle's extra top row holds the numbers instead.

| Layer | Box switch | Right-half contents |
| --- | --- | --- |
| `_BASE` | n/a | Corne right half; top row `6 7 8 9 0 ``. Thumbs: `LT(_SYM,BSPC)`, Space, OS-Alt, Enter, Tab (Enter/Tab are placeholders) |
| `_LEFT` | left (F6) = `OSL(_LEFT)` | Corne left-hand letters in their normal order (not mirrored), shifted one column left: `1-5`, `Q-T`, `A-G`, `Z-B` in R0–R4; R5 is transparent (base layer's Esc / Del / Shift / Ctrl). The Corne's outer column (`` ` `` Esc Shift Ctrl) was dropped |
| `_SYM` | middle (F7) = `OSL(_SYM)` | Right half of the Corne symbol layer |
| `_FN` | right (B1) = `OSL(_FN)` | F1–F6 top row, F7–F12 below. Arrows as an inverted T (Up at R2, Left/Down/Right at R1–R3 below), Ctrl+Tab left of Up, Ctrl+Shift+Tab right of Up. LED keys: `RM_NEXT` (pattern) / `RM_TOGG` (on/off) at R0, `RM_VALU`/`RM_VALD` (brightness) at R4. The Corne's dynamic-macro/clipboard keys were removed by the user |
| n/a | B3 = `KC_NO` | Unfitted position |

- `OSL()` = tap for the next key only, hold for momentary. The user chose this behaviour.
- `keymap.c` formatting: each layer is a fixed grid of 12-character columns (labelled L0–L5, inner, inner, R0–R5), wrapped in `// clang-format off/on`. Keep new keycodes within 11 characters; use a `#define` alias for longer ones, as with `SYM_BSPC` = `LT(_SYM, KC_BSPC)`. The grid was produced by a one-off script; hand edits are fine as long as columns stay aligned.
- Combos: Y+U `)`, H+J `]`, N+M `}`, J+K Esc on base; R+T `(`, F+G `[`, V+B `{` on `_LEFT`.
- The right rotary encoder uses the board default (PgUp/PgDn).
- **LED brightness is capped** at `RGB_MATRIX_MAXIMUM_BRIGHTNESS 130` (default 50; raised from 100, which was stable but dim; 130 confirmed stable and bright enough by the user) in `config.h`, and `keyboard_post_init_user()` clamps any saved brightness above that at boot. Without the cap (QMK's default is 255), changing LED pattern browned out the board: USB disconnected, the LEDs went dark, and it boot-looped. Each crash-reset counted toward the double-tap-reset detection (timeout raised to 2000 ms by the user), so it kept dropping into the bootloader (the RPI-RP2 drive opening/closing in Explorer). Recovery took pulling the controller and shorting reset. The board's own default keymap uses the same cap of 100. The controller and touchpad sit in removable pin sockets; those add resistance and can flex, so suspect them if resets happen at low brightness.
- **Reactive LED patterns** (keys light up when pressed; wide/cross/nexus/splash variants light the keys around them) are enabled in `config.h` with `RGB_MATRIX_KEYPRESSES` plus 12 `ENABLE_RGB_MATRIX_*` lines. `RM_NEXT` cycles 41 patterns in total; delete lines to trim. These don't change the split transport, so only the right half needs flashing.
- Bootmagic: holding the right half's top outer key (Esc, [5,0]) while plugging in wipes saved settings and enters the bootloader. It's a power-on check, not a keymap key.
- Per-key RGB matrix is on (board default, 29 patterns enabled in `sofle_choc/keyboard.json`); the `RM_*` keys on `_FN` save their settings to EEPROM automatically.
- **Touchpad scroll speed:** `TOUCHPAD_SCROLL_DIVISOR` in `config.h` (whole number; higher = slower; 1 = raw). It's applied in `pointing_device_task_user()` in `keymap.c`, with a carry so slow scrolls aren't lost. To scroll *faster*, use Windows Settings → Bluetooth & devices → Mouse → "Lines to scroll at a time" (affects all mice). The QMK Azoteq driver has no built-in scroll-speed option.

## Open items / known gaps

1. Arrows and Ctrl(+Shift)+Tab are now on `_FN`. Still missing from the Corne: Home/End, PgUp/PgDn (the encoder does PgUp/PgDn), cut/copy/paste, and `QK_REP`.
2. **Left half of the Corne symbol layer** (`^ = `` @ * - # + _`) was never ported.
3. Box layer keys were just restored on F6/F7/B1; the user is testing them. Their wiring was confirmed with a digit test first.
4. Typing several left-hand letters needs SW1 held or tapped before each letter. Option: `#define ONESHOT_TAP_TOGGLE 2` so a double-tap locks the layer.
5. The 180° touchpad rotation is confirmed.
6. Find out whether she can comfortably hold the buttons. This affects whether `OSL` is the right choice versus `TG`, Layer Lock, etc.
7. Backup right-hand-only ways to change layers (layer-taps, combos, encoder) were in the original plan; only `LT(_SYM, BSPC)` exists so far.
8. Everything since commit 49e79dbecd ("Adding initial keymap") is uncommitted.
9. `RGB_MATRIX_MAXIMUM_BRIGHTNESS` was later raised to 200 by the user, above the 130 tested as stable. Drop it back if resets return.

## Related

A design doc with the wiring diagram exists as a Claude Doc, titled "Sofle left-half button box wiring".
