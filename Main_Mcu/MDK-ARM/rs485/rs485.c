#include "rs485.h"

uint8_t tx_buf[RS485_MAX_FRAME_LEN];
uint8_t rx_buf[RS485_MAX_FRAME_LEN];

void RS485_Master_SendReadCmd(uint8_t slave_addr) {
    RS485_Frame_t frame;
    frame.head = RS485_FRAME_HEAD;
    frame.addr_to = slave_addr;
    frame.addr_from = RS485_ADDR_MASTER;
    frame.cmd = CMD_READ_DATA;
    frame.len = 0;
    frame.checksum = RS485_CalcChecksum((uint8_t*)&frame, 5);

    memcpy(tx_buf, &frame, 6); 
    HAL_UART_Transmit(&huart3, tx_buf, 6, 100);
}
uint8_t RS485_CalcChecksum(const uint8_t *buf, uint8_t len) 
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }
    return (uint8_t)sum;
}