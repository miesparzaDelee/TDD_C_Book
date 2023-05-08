#ifndef IO_FLASH_H
#define IO_FLASH_H
#include <stdint.h>

typedef uint32_t ioAddress;
typedef uint16_t ioData;

void IOF_Write(ioAddress addr, ioData data);

ioData IOF_Read(ioAddress addr);

#endif
