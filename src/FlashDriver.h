#ifndef FLASH_DRIVER_H
#define FLASH_DRIVER_H
#include <stdint.h>
#include "IO_Flash.h"

#define FD_SUCCESS 0
#define FD_TIMEOUT 1
#define FD_VPP_ERROR 2
#define FD_PROGRAM_ERROR 3
#define FD_PROTECTED_WRITE_ERROR 4
#define FD_READBACK_ERROR 5


int FD_Program(ioAddress address, ioData data);

#endif