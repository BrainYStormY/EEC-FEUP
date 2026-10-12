#include <stdio.h>
#include <stdint.h>
/*
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
*/

#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 5502

int main(){
    uint16_t StartingCoilAddr = 1;
    uint16_t NumCoils = 8;
    uint8_t writeVal[1] = {0xAB};
    uint8_t readVal[256] = {0};

    printf("---Testing Write Multiple Coils---\n");
    int responseWrite = Write_coils(SERVER_ADDR, SERVER_PORT, StartingCoilAddr, NumCoils, writeVal);
    if(responseWrite > 0) printf("%d Coils Written 0x%02X Successfully!\n", responseWrite, writeVal[0]);
    else printf("Error Writting Coils!\n");

    printf("\n---Testing Read Coils---\n");
    int responseRead = Read_coils(SERVER_ADDR, SERVER_PORT, StartingCoilAddr, NumCoils, readVal);
    if(responseRead > 0) printf("%d Coils Read Successfully! Value: 0x%02X\n", responseRead, readVal[0]);
    else printf("Error Reading Coils!\n");

    return 0;
}