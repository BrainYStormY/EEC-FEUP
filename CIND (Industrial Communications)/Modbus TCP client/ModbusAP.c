#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "ModbusAP.h"
#include "ModbusTCP.h"


// Read Coils (Function Code 0x01)
int Read_coils(char *server_add, int port, uint16_t StartingCoilAddr, uint16_t NumCoils, uint8_t *val){
    if(NumCoils == 0 || val == NULL) return -1; // Invalid parameters

    uint8_t APDU[5];            // 1 for FC + 2 for StartAdd + 2 for NumCoils
    uint8_t received_APDU[252]; // 1 for FC + 1 for ByteCount + 250 for Data (max 2000 coils = 250 bytes)
    uint16_t startCoilAdd = StartingCoilAddr - 1;

    // Construct APDU
    APDU[0] = 0x01;                             // FC for Read Coils
    APDU[1] = (startCoilAdd >> 8) & 0xFF;       // Starting Coil Address High byte
    APDU[2] = startCoilAdd & 0xFF;              // Starting Coil Address Low byte
    APDU[3] = (NumCoils >> 8) & 0xFF;           // Number of Coils High byte
    APDU[4] = NumCoils & 0xFF;                  // Number of Coils Low byte

    // Send the request
    int response_len = Send_Modbus_request(server_add, port, APDU, sizeof(APDU), received_APDU);
    if(response_len < 0) return -1; // Error in sending request

    // Verify Exception in Modbus
    if(received_APDU[0] & 0x80){
        printf("Modbus Exception Code: 0x%02X\n", received_APDU[1]);
        return -1;  // Exception response
    }

    uint8_t byteCount = received_APDU[1];
    memcpy(val, &received_APDU[2], byteCount);  // Copy the coil values to val

    return NumCoils;    // Return the number of coils read
}


// Write Multiple Coils (Function Code 0x0F)
int Write_coils(char *server_add, int port, uint16_t StartingCoilAddr, uint16_t NumCoils, uint8_t *val){
    if(NumCoils == 0 || val == NULL) return -1; // Invalid parameters

    uint8_t byte_count = NumCoils / 8;  // Calculate the number of bytes needed to represent the coils
    if(NumCoils % 8 != 0) byte_count++; // If there are remaining bits, add one more byte
    uint8_t APDU[256];                  // 1 for FC + 2 for StartAdd + 2 for NumCoils + 1 for ByteCount + 250 for Data (max 2000 coils = 250 bytes)
    uint8_t received_APDU[5];           // 1 for FC + 2 for StartAdd + 2 for NumCoils
    uint16_t startCoilAdd = StartingCoilAddr - 1;

    // Construct APDU
    APDU[0] = 0x0F;                         // FC for Write Multiple Coils
    APDU[1] = (startCoilAdd >> 8) & 0xFF;   // Starting Coil Address High byte
    APDU[2] = startCoilAdd & 0xFF;          // Starting Coil Address Low byte
    APDU[3] = (NumCoils >> 8) & 0xFF;       // Number of Coils High byte
    APDU[4] = NumCoils & 0xFF;              // Number of Coils Low byte
    APDU[5] = byte_count;                   // Byte count

    memcpy(&APDU[6], val, byte_count);  // Copy the coil values to APDU
    int APDU_len = 6 + byte_count;      // Total length of APDU

    // Send the request
    int response_len = Send_Modbus_request(server_add, port, APDU, APDU_len, received_APDU);
    if(response_len < 0) return -1; // Error in sending request

    // Verify Exception in Modbus
    if(received_APDU[0] & 0x80){
        printf("Modbus Exception Code: 0x%02X\n", received_APDU[1]);
        return -1;  // Exception response
    }

    return NumCoils;    // Return the number of coils written
}
