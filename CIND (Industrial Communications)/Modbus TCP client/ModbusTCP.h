#ifndef MODBUS_TCP_H
#define MODBUS_TCP_H

#include <stdint.h>

int Send_Modbus_request(char *server_ip, int port, uint8_t *APDU, int APDU_len, uint8_t *response_APDU);

#endif
