#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "ModbusTCP.h"


uint16_t transaction_id = 1;


int Send_Modbus_request(char *server_ip, int port, uint8_t *APDU, int APDU_len, uint8_t *response_APDU){

    // Create TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("Socket creation failed");
        return -1;
    }

    // Configure server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0){
        perror("Invalid address");
        close(sock);
        return -1;
    }

    // Connect to server
    if(connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Connection failed");
        close(sock);
        return -1;
    }

    // Construct PDU (MBAPH + APDU) // Program Data Unit = ModBus Application Protocol Header + Application Protocol Data Unit
        // MBAPH (7 bytes):
            // TI - Transaction ID (2 bytes)
            // PI - Protocol ID (2 bytes)   0 for Modbus TCP
            // L - Length (2 bytes)
            // UI - Unit ID (1 byte)
        // APDU (1 + N bytes):
            // FC - Function Code (1 byte)
            // D - Data (N bytes)

    // PDU (7 + 1 + N bytes):
    uint8_t PDU[258];   // 7 for header + 1 for FC + 250 for Data (max 2000 coils = 250 bytes)
    uint16_t current_ti = transaction_id++;
    uint16_t lenght = APDU_len + 1;  // Length of APDU + Unit ID

    PDU[0] = (current_ti >> 8) & 0xFF;  // TI high byte
    PDU[1] = current_ti & 0xFF;         // TI low byte
    PDU[2] = 0x00;                      // PI High byte
    PDU[3] = 0x00;                      // PI Low byte
    PDU[4] = (lenght >> 8) & 0xFF;      // L High byte
    PDU[5] = lenght & 0xFF;             // L Low byte
    PDU[6] = 0x01;                      // UI (0x01 for default)

    // Copy APDU into PDU
    memcpy(&PDU[7], APDU, APDU_len);
    int PDU_len = 7 + APDU_len;

    // Send TCP message
    if(send(sock, PDU, PDU_len, 0) < 0){
        perror("Send failed");
        close(sock);
        return -1;
    }

    // Receive response from server
    uint8_t response_PDU[7 + MAX_DATA_BYTE_COUNT];
    int bytes_received = recv(sock, response_PDU, sizeof(response_PDU), 0);
    if(bytes_received < 7){
        perror("Invalid response or reception error");
        close(sock);
        return -1;
    }

    // Extract APDU from response (discard MBAPH)
    int response_APDU_len = bytes_received - 7;
    memcpy(response_APDU, &response_PDU[7], response_APDU_len);

    // Close socket
    close(sock);

    return response_APDU_len;  // Return length of received APDU
}
