/*
 * File: protocol.h
 * Author: Ted Salmon <tass2001@gmail.com>
 * Description:
 *     Implement a simple message protocol for the bootloader
 */
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include "../mappings.h"
#include "flash.h"
#include "uart.h"
#define PROTOCOL_MAX_DATA_SIZE 255
#define PROTOCOL_CONTROL_PACKET_SIZE 3
#define PROTOCOL_PACKET_STATUS_BAD 0
#define PROTOCOL_PACKET_STATUS_INCOMPLETE 1
#define PROTOCOL_PACKET_STATUS_OK 3
/**
 * Commands that the bootloader recognizes
 */
#define PROTOCOL_CMD_PLATFORM_REQUEST 0x00
#define PROTOCOL_CMD_PLATFORM_RESPONSE 0x01
#define PROTOCOL_CMD_VERSION_REQUEST 0x02
#define PROTOCOL_CMD_VERSION_RESPONSE 0x03
#define PROTOCOL_CMD_WRITE_DATA_REQUEST 0x04
#define PROTOCOL_CMD_WRITE_DATA_RESPONSE_OK 0x05
#define PROTOCOL_CMD_WRITE_DATA_RESPONSE_ERR 0x06
#define PROTOCOL_CMD_START_APP_REQUEST 0x07
#define PROTOCOL_CMD_START_APP_RESPONSE 0x08
#define PROTOCOL_BAD_PACKET_RESPONSE 0xFF

/**
 * Packet Description:
 * <Command Byte> <Length of entire message> <data (up to 128 bytes)> <XOR>
 * At least _one_ data byte is required, though it can be any value
 *
 * Packet integrity is verified by the XOR byte at the end of the packet
 */

/**
 * ProtocolPacket_t
 *     Description:
 *         This object defines the structure of a protocol packet
 */
typedef struct ProtocolPacket_t {
    uint8_t status;
    unsigned char command;
    uint8_t dataSize;
    unsigned char data[PROTOCOL_MAX_DATA_SIZE];
} ProtocolPacket_t;

void ProtocolFlashWrite(UART_t *, ProtocolPacket_t *);
ProtocolPacket_t ProtocolProcessPacket(UART_t *);
void ProtocolSendPacket(UART_t *, unsigned char, unsigned char *, uint8_t);
void ProtocolSendStringPacket(UART_t *, unsigned char, char *);
uint8_t ProtocolValidatePacket(ProtocolPacket_t *, unsigned char);

#endif /* PROTOCOL_H */
