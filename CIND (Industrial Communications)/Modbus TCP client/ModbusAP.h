#ifndef MODBUSAP_H
#define MODBUSAP_H

#include <stdint.h>

// Function 01: Read Coils
int Read_coils(char *server_add, int port, uint16_t StartingCoilAddr, uint16_t NumCoils, uint8_t *val);

// Function 15 (0x0F): Write Multiple Coils
int Write_coils(char *server_add, int port, uint16_t StartingCoilAddr, uint16_t NumCoils, uint8_t *val);

#endif
