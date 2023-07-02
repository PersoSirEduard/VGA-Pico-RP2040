#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ----- //
// morse //
// ----- //

#define morse_wrap_target 0
#define morse_wrap 2

static const uint16_t morse_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block                      
    0x6001, //  1: out    pins, 1                    
    0x0000, //  2: jmp    0                          
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program morse_program = {
    .instructions = morse_program_instructions,
    .length = 3,
    .origin = -1,
};

static inline pio_sm_config morse_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + morse_wrap_target, offset + morse_wrap);
    return c;
}

static inline void morse_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = morse_program_get_default_config(offset);
    // Map the state machine's OUT pin group to one pin, namely the `pin`
    // parameter to this function.
    sm_config_set_out_pins(&c, pin, 1);
    // Set this pin's GPIO function (connect PIO to the pad)
    pio_gpio_init(pio, pin);
    // Set the pin direction to output at the PIO
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    // Load our configuration, and jump to the start of the program
    pio_sm_init(pio, sm, offset, &c);
    // Set the state machine running
    pio_sm_set_enabled(pio, sm, true);
}

#endif
