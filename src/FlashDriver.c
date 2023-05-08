#include "FlashDriver.h"
#include "IO_Flash.h"

int FD_Program(uint16_t address, uint8_t data)
{
	int nReads = 0;
	IOF_Write(0, 0x40);
	IOF_Write(address, data);
	uint8_t status = IOF_Read(0);
	while (!(status & (1 << 7)))
	{
		status = IOF_Read(0);
		nReads++;
		if (nReads == 10)
			return -1;
	}
	uint8_t rdata = IOF_Read(address);
	return FD_SUCCESS;
}