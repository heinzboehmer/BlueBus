/*
 * File:   utils.c
 * Author: Ted Salmon <tass2001@gmail.com>
 * Description:
 *     Helper utils that may be useful in more than one place
 */
#include "utils.h"

/* Hold a pin to register map for all programmable output pins */
static uint16_t *ROPR_PINS[] = {
    GET_RPOR(0),
    GET_RPOR(0),
    GET_RPOR(1),
    GET_RPOR(1),
    GET_RPOR(2),
    GET_RPOR(2),
    GET_RPOR(3),
    GET_RPOR(3),
    GET_RPOR(4),
    GET_RPOR(4),
    GET_RPOR(5),
    GET_RPOR(5),
    GET_RPOR(6),
    GET_RPOR(6),
    GET_RPOR(7),
    GET_RPOR(7),
    GET_RPOR(8),
    GET_RPOR(8),
    GET_RPOR(9),
    GET_RPOR(9),
    GET_RPOR(10),
    GET_RPOR(10),
    GET_RPOR(11),
    GET_RPOR(11),
    GET_RPOR(12),
    GET_RPOR(12),
    GET_RPOR(13),
    GET_RPOR(13),
    GET_RPOR(14),
    GET_RPOR(14),
    GET_RPOR(15),
    GET_RPOR(15),
    GET_RPOR(16),
    GET_RPOR(16),
    GET_RPOR(17),
    GET_RPOR(17),
    GET_RPOR(18),
    GET_RPOR(18)
};

UtilsAbstractDisplayValue_t UtilsDisplayValueInit(char *text, uint8_t status)
{
    UtilsAbstractDisplayValue_t value;
    strncpy(value.text, text, UTILS_DISPLAY_TEXT_SIZE - 1);
    value.index = 0;
    value.timeout = 0;
    value.status = status;
    value.length = strlen(text);
    return value;
}

/**
 * UtilsNormalizeText()
 *     Description:
 *         Unescape characters and convert them from UTF-8 to their Unicode
 *         bytes. This is to support extended ASCII.
 *     Params:
 *         char *string - The subject
 *         const char *input - The string to copy from
 *     Returns:
 *         void
 */
void UtilsNormalizeText(char *string, const char *input)
{
    uint16_t idx;
    uint16_t strIdx = 0;
    uint16_t strLength = strlen(input);
    for (idx = 0; idx < strLength; idx++) {
        uint32_t c = input[idx];
        if (c == 0x5C) {
            uint16_t byteIdx = idx;
            uint8_t byteSize = 0;
            uint8_t curByte = input[byteIdx];
            while (curByte == 0x5C && byteSize <= 3) {
                if (idx + 3 <= strLength) {
                    byteSize = byteSize + 1;
                    byteIdx = byteIdx + 3;
                    idx = idx + 3;
                    curByte = input[byteIdx];
                } else {
                    curByte = 0x00;
                }
            }
            if (curByte != 0x00) {
                // Bring the index back one so we're on the "current" index still
                idx--;
            }
            uint8_t byteOffset = 0;
            unsigned char rawUtf[] = {0x00, 0x00, 0x00};
            while (byteSize > 0) {
                char buf[] = {input[byteIdx - 2], input[byteIdx - 1], '\0'};
                unsigned char byte = UtilsStrToHex(buf);
                rawUtf[2 - byteOffset] = byte;
                byteOffset++;
                byteIdx = byteIdx - 3;
                byteSize--;
            }
            c = (
                ((uint32_t)0 << 24) + // "Phantom" Byte
                ((uint32_t)rawUtf[0] << 16) +
                ((uint32_t)rawUtf[1] << 8) +
                ((uint32_t)rawUtf[2])
            );
        }
        if (c >= 0x20 && c <= 0x7E) {
            string[strIdx] = (uint8_t) c;
            strIdx++;
        } else if (c > 0x7E) {
            // Convert UTF-8 byte to Unicode then check if it falls within
            // the range of extended ASCII
            c = c - 0xC2C0;
            if (c < 0xFF) {
                string[strIdx] = (uint8_t) c & 0xFF;
                strIdx++;
            }
        }
    }
    string[strIdx] = '\0';
}

/**
 * UtilsRemoveSubstring()
 *     Description:
 *         Remove the given substring from the given subject
 *     Params:
 *         char *string - The subject
 *         const char *trash - The substring to remove
 *     Returns:
 *         void
 */
void UtilsRemoveSubstring(char *string, const char *trash)
{
    uint16_t removeLength = strlen(trash);
    while ((string = strstr(string, trash))) {
        memmove(string, string + removeLength, 1 + strlen(string + removeLength));
    }
}

/**
 * UtilsReset()
 *     Description:
 *         Reset the MCU
 *     Params:
 *         void
 *     Returns:
 *         void
 */
void UtilsReset()
{
    __asm__ volatile ("RESET");
}

/**
 * UtilsSetRPORMode()
 *     Description:
 *         Set the mode of a programmable output pin
 *     Params:
 *         uint8_t pin - The pin to set
 *         uint8_t mode - The mode to set the given pin to
 *     Returns:
 *         void
 */
void UtilsSetRPORMode(uint8_t pin, uint16_t mode)
{
    if ((pin % 2) == 0) {
        uint16_t msb = *ROPR_PINS[pin] >> 8;
        // Set the least significant bits for the even pin number
        *ROPR_PINS[pin] = (msb << 8) + mode;
    } else {
        uint16_t lsb = *ROPR_PINS[pin] & 0xFF;
        // Set the least significant bits of the register for the odd pin number
        *ROPR_PINS[pin] = (mode << 8) + lsb;
    }
}

/**
 * UtilsStrToHex()
 *     Description:
 *         Convert a string to a octal
 *     Params:
 *         char *string - The subject
 *     Returns:
 *         uint8_t The unsigned char
 */
unsigned char UtilsStrToHex(char *string)
{
    char *ptr;
    return (unsigned char) strtol(string, &ptr, 16);
}


/**
 * UtilsStrToInt()
 *     Description:
 *         Convert a string to an integer
 *     Params:
 *         char *string - The subject
 *     Returns:
 *         uint8_t The Unsigned 8-bit integer representation
 */
uint8_t UtilsStrToInt(char *string)
{
    char *ptr;
    return (uint8_t) strtol(string, &ptr, 10);
}

/**
 * UtilsStricmp()
 *     Description:
 *         Case-Insensitive string comparison 
 *     Params:
 *         const char *string - The subject
 *         const char *compare - The string to compare the subject against
 *     Returns:
 *         int8_t -
 *             Negative 1 when string is less than compare
 *             Zero when string matches compare
 *             Positive 1 when string is greater than compare
 */
int8_t UtilsStricmp(const char *string, const char *compare)
{
    int8_t result;
    while(!(result = toupper(*string) - toupper(*compare)) && *string) {
        string++;
        compare++;
    }
    return result;
}
