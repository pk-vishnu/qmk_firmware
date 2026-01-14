/*
 * Copyright 2025 Lazercore
 * Author: P K, Vishnu
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "type_alchemy.h"
#include "quantum.h"
#include <string.h>
#include <stdlib.h>
#include "send_string.h"

static bool type_alchemy_active = false;
static char typed_buffer[MAX_BUFFER_SIZE]= "" ;
static uint8_t buffer_index = 0;

static WordSymbolMapping mappings[MAX_MAPPING_SIZE];
static size_t mapping_count = 0;

static const WordSymbolMapping default_mappings[] = {
    {"aum", "ॐ"},
    {"inr", "₹"},
    {"pi", "π"},
    {"degree", "°"},
    {"micro", "µ"},
    {"integral", "∫"},
    {"infinity", "∞"},
    {"sigma", "Σ"},
    {"delta", "Δ"},
    {"theta", "θ"},
    {"alpha", "α"},
    {"beta", "β"},
    {"gamma", "γ"},
    {"lambda", "λ"},
    {"omega", "Ω"},
    {"sqrt", "√"},
    {"not_equal", "≠"},
    {"less_equal", "≤"},
    {"greater_equal", "≥"},
    {"approx", "≈"},
    {"arrow_right", "→"},
    {"arrow_left", "←"},
    {"arrow_up", "↑"},
    {"arrow_down", "↓"},
    {"percent", "%"},
    {"times", "×"},
    {"divide", "÷"},
    {"ellipsis", "…"},
    {"caret", "^"},
    {"logical_and", "∧"},
    {"logical_or", "∨"},
    {"perpendicular", "⊥"},
    {"parallel", "∥"},
    {"smile", "😊"},
    {"heart", "❤️"},
    {"thumbs_up", "👍"},
    {"fire", "🔥"},
};


static const size_t default_mapping_count = sizeof(default_mappings)/sizeof(default_mappings[0]);

void init_type_alchemy(void){
    type_alchemy_active = false;
    buffer_index = 0;
    memset(typed_buffer, 0, MAX_BUFFER_SIZE);
    mapping_count = default_mapping_count;
    for (size_t i = 0; i < default_mapping_count; i++) {
        mappings[i] = default_mappings[i];
    }
}

bool add_word_symbol_mapping(const char *word, const char *symbol) {
    if (mapping_count >= MAX_MAPPING_SIZE) {
        return false; // Mapping array is full
    }
    mappings[mapping_count].word = word;
    mappings[mapping_count].symbol = symbol;
    mapping_count++;
    return true;
}
void toggle_type_alchemy(void){
    type_alchemy_active = !type_alchemy_active;
}
bool is_type_alchemy_on(void){
    return type_alchemy_active;
}
bool process_type_alchemy(uint16_t keycode, char typed_char, bool is_pressed) {
    if (!type_alchemy_active || !is_pressed) {
        return true; // Pass through if the feature is off or the key is released
    }

    if ((typed_char >= 'a' && typed_char <= 'z') || (typed_char >= 'A' && typed_char <= 'Z')) {
        if (buffer_index < sizeof(typed_buffer) - 1) {
            typed_buffer[buffer_index++] = typed_char;
            typed_buffer[buffer_index] = '\0'; // Null-terminate the string
        }

        // Check if the buffer matches any mapping
        for (size_t i = 0; i < mapping_count; i++) {
            if (strcmp(typed_buffer, mappings[i].word) == 0) {
                if (mappings[i].symbol != NULL) {
                    for (size_t j = 0; j < strlen(mappings[i].word)-1; j++) {
                        register_code(KC_BACKSPACE);
                    }
                    send_unicode_string(mappings[i].symbol);
                }

                buffer_index = 0;
                memset(typed_buffer, 0, sizeof(typed_buffer));
                return false;
            }
        }
    } else {
        buffer_index = 0;
        memset(typed_buffer, 0, sizeof(typed_buffer));
    }

    return true;
}
