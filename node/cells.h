#ifndef __CELLS__
#define __CELLS__

#include <stdint.h>

// Control cell commands
#define PADDING 0x01
#define CREATE 0x02
#define CREATED 0x03
#define DESTROY 0x04

// Relay flag
#define RELAY 0x05

typedef struct ControlCell {
    uint16_t circ_id;
    uint8_t cmd;
    uint8_t data[509];
} ControlCell;

typedef struct RelayCell {
    uint16_t circ_id;
    uint16_t stream_id;
    uint8_t digest[6];
    uint16_t len;
    uint8_t cmd;
    uint8_t data[498];
} RelayCell;

int is_relay_cell(uint8_t *cell);
ControlCell *new_control_cell(uint8_t *cell);
RelayCell *new_relay_cell(uint8_t *cell);
uint8_t *control_cell_to_byte(ControlCell *cell);
uint8_t *relay_cell_to_byte(RelayCell *cell);

#endif
