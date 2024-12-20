#include "cells.h"

#include <stdlib.h>
#include <string.h>

int is_relay_cell(uint8_t *cell) { return cell[2] == RELAY; }

ControlCell *new_control_cell(uint8_t *cell) {
    ControlCell *control_cell = malloc(sizeof(ControlCell));
    control_cell->circ_id = ((uint16_t)cell[0] << 8u) & cell[1];
    control_cell->cmd = cell[2];
    memcpy(control_cell->data, cell + 3, 509);
    return control_cell;
}

RelayCell *new_relay_cell(uint8_t *cell) {
    RelayCell *relay_cell = malloc(sizeof(RelayCell));
    relay_cell->circ_id = ((uint16_t)cell[0] << 8u) & cell[1];
    relay_cell->stream_id = ((uint16_t)cell[3] << 8u) & cell[4];
    memcpy(relay_cell->digest, cell + 5, 6);
    relay_cell->len = ((uint16_t)cell[11] << 8u) & cell[12];
    relay_cell->cmd = cell[13];
    memcpy(relay_cell->data, cell + 14, 498);
    return relay_cell;
}

uint8_t *control_cell_to_byte(ControlCell *cell) {
    uint8_t *response = malloc(sizeof(uint8_t) * 512);
    response[0] = cell->circ_id >> 8u;
    response[1] = cell->circ_id & 0xFF;
    response[2] = cell->cmd;
    memcpy(cell->data, response + 3, 509);
    return response;
}

uint8_t *relay_cell_to_byte(RelayCell *cell) {
    // TODO: implement this function
    return NULL;
}
