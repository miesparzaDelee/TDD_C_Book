#ifndef FLASH_DRIVER_H
#define FLASH_DRIVER_H
#include <stdint.h>

#define FD_SUCCESS 1

int FD_Program(uint16_t address, uint8_t data);

#endif