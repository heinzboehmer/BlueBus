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

/**
 * removeNonAscii()
 *     Description:
 *         Ignore non-ASCII characters from input and put the rest into string
 *     Params:
 *         char *string - The subject
 *         const char *input - The string to copy from
 *     Returns:
 *         void
 */
void removeNonAscii(char *string, const char *input)
{
    uint16_t idx;
    uint16_t strIdx = 0;
    for (idx = 0; idx < strlen(input); idx++) {
        char c = input[idx];
        if (c >= 0x20 && c <= 0x7E) {
            string[strIdx] = c;
            strIdx++;
        }
    }
    string[strIdx] = '\0';
}

/**
 * removeSubstring()
 *     Description:
 *         Remove the given substring from the given subject
 *     Params:
 *         char *string - The subject
 *         const char *trash - The substring to remove
 *     Returns:
 *         void
 */
void removeSubstring(char *string, const char *trash)
{
    uint16_t removeLength = strlen(trash);
    while((string = strstr(string, trash) )){
        memmove(string, string + removeLength, 1 + strlen(string + removeLength));
    }
}

/**
 * setRPORMode()
 *     Description:
 *         Set the mode of a programmable output pin
 *     Params:
 *         uint8_t pin - The pin to set
 *         uint8_t mode - The mode to set the given pin to
 *     Returns:
 *         void
 */
void setRPORMode(uint8_t pin, uint16_t mode)
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
 * strToInt()
 *     Description:
 *         Convert a string to an integer
 *     Params:
 *         char *string - The subject
 *     Returns:
 *         uint8_t The Unsigned 8-bit integer representation
 */
uint8_t strToInt(char *string)
{
    char *ptr;
    return (uint8_t) strtol(string, &ptr, 10);
}
