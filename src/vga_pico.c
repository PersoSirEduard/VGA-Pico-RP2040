#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "morse.pio.h"
#include "hardware/dma.h"

#define WIDTH 640
#define HEIGHT 480

int main() {
#ifndef PICO_DEFAULT_LED_PIN
#warning program requires a board with a regular LED
#else

    unsigned char data = 1;
    char *pData = &data;

    PIO pio = pio0; // Instance of PIO Block 0

    uint offset = pio_add_program(pio, &morse_program); // Load the program
    uint sm = pio_claim_unused_sm(pio, true); // Claim a free state machine
    morse_program_init(pio, sm, offset, PICO_DEFAULT_LED_PIN);

    uint dma_chan = dma_claim_unused_channel(true);
    dma_channel_config dma_cfg = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, false);
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8);
    channel_config_set_dreq(&dma_cfg, pio_get_dreq(pio, sm, true));
    dma_channel_configure(dma_chan, &dma_cfg,
                          &pio->txf[sm],    // PIO TX FIFO
                          pData,            // Data buffer
                          sizeof(data),     // Buffer length
                          false);           // Start now

    while (true)
    {
        // pio_sm_put_blocking(pio, sm, 1);
        // sleep_ms(1000);
        // pio_sm_put_blocking(pio, sm, 0);
        // sleep_ms(1000);
        data = 0;
        dma_channel_start(dma_chan);
        sleep_ms(1000);
        dma_channel_wait_for_finish_blocking(dma_chan);
        data = 1;
        dma_channel_start(dma_chan);
        dma_channel_wait_for_finish_blocking(dma_chan);
        sleep_ms(1000);
    }

#endif
}