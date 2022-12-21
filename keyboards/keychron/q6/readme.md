# Keychron Q6

A customizable 100% keyboard.

* Keyboard Maintainer: [Keychron](https://github.com/keychron)
* Hardware Supported: Keychron Q6
* Hardware Availability: [Keychron](https://www.keychron.com)

Make example for this keyboard (after setting up your build environment):

    make keychron/q6/ansi:default

Flashing example for this keyboard:

    make keychron/q6/ansi:default:flash

**Reset Key**: Hold down the key located at *K00*, commonly programmed as *Esc* while plugging in the keyboard.

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).


File overview:
. q6.c
    dip switch, caps/num indicator

. config.h
    manage led animations 

. iso_encoder/rules.mk
    toggle console, debug

. iso_encoder/iso_encoder.c
    led matrix, key type

. iso_encoder/config.h
    define caps/num led, encoder resolution

. iso_encoder/rules.mk
    toggle console, command

. iso_encoder/keymaps/mrtnritter/keymap.c
    the keymap

. common/keychron_common.h
    custom keycodes definition

. common/keychron_common.c
    custom keycodes functions, housekeeping
