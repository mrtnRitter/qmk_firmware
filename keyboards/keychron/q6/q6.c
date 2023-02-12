/* Copyright 2022 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "q6.h"
#include "print.h"  // for debugging

const matrix_row_t matrix_mask[] = {
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111111111,
    0b11111111111111101111,
};

// DIP SWITCH
#ifdef DIP_SWITCH_ENABLE

bool dip_switch_update_kb(uint8_t index, bool active) {
    if (!dip_switch_update_user(index, active)) {
        return false;
    }
    if (index == 0) {
        if (active) {
            reset_keyboard();
        } else {
            default_layer_set(0);
        }
    }
    return true;
}
#endif 

// RGB MATRIX ENABLED
#if defined(RGB_MATRIX_ENABLE)

// GLOBALS
uint32_t layer_switch_time;
uint8_t  layer_indicator_led;


// EVENT on layer change - also runs on init
layer_state_t layer_state_set_kb(layer_state_t state) {
    layer_switch_time   = timer_read32();
    rgb_matrix_set_color(layer_indicator_led, 0, 0, 0);
    layer_indicator_led = 16 + get_highest_layer(state);

    return state;
}

// RGB TOGGLE
bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) {
        return false;
    }
    switch (keycode) {
        case RGB_TOG:
            if (record->event.pressed) {
                switch (rgb_matrix_get_flags()) {
                    case LED_FLAG_ALL: {
                        rgb_matrix_set_flags(LED_FLAG_NONE);
                        rgb_matrix_set_color_all(0, 0, 0);
                    } break;
                    default: {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                    } break;
                }
            }
            if (!rgb_matrix_is_enabled()) {
                rgb_matrix_set_flags(LED_FLAG_ALL);
                rgb_matrix_enable();
            }
            return false;
    }
    return true;
}

// INDICATORS - runs 60 times/second
bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) {
        return false;
    }

    // Caps lock indicator
    #if defined(CAPS_LOCK_LED_INDEX)
    if (host_keyboard_led_state().caps_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 255, 255, 255);
    } else {
        if (!rgb_matrix_get_flags()) {
            RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 0, 0, 0);
        }
    }
    #endif

    // Num lock indicator
    #if defined(NUM_LOCK_LED_INDEX)
    if (host_keyboard_led_state().num_lock) {
        RGB_MATRIX_INDICATOR_SET_COLOR(NUM_LOCK_LED_INDEX, 255, 255, 255);
    } else {
        if (!rgb_matrix_get_flags()) {
            RGB_MATRIX_INDICATOR_SET_COLOR(NUM_LOCK_LED_INDEX, 0, 0, 0);
        }
    }
    #endif

    // Layer indicator

    if (timer_elapsed32(layer_switch_time) <= 1000) {

        uint8_t time_to_val = 0.255f * timer_elapsed32(layer_switch_time); // 0 -> 255
        HSV     hsv         = rgb_matrix_config.hsv;

        if (rgb_matrix_get_flags()) {
            if (layer_indicator_led != 19) {
                hsv.h += rgb_matrix_config.speed;
            }

            hsv.s = time_to_val;

        } else {
            hsv.h = 0;
            hsv.s = 0;
            hsv.v = 255 - time_to_val;
        }

        RGB rgb = hsv_to_rgb(hsv);
        rgb_matrix_set_color(layer_indicator_led, rgb.r, rgb.g, rgb.b);

    }
    return true;
}
#endif
