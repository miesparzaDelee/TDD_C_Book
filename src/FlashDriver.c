#include "FlashDriver.h"
#include <stdbool.h>

static bool TestStatusRegError(ioData statusReg, int error);
static bool TestByteSet(uint32_t reg, int byte);

typedef enum
{
	Command_Register = 0x0,
	Status_Register = 0x0
}Flash_Registers;

typedef enum
{
	ProgramCommand = 0x40,
	ResetCommand = 0xff
} Flash_Command;

int FD_Program(ioAddress address, ioData data)
{
	int nReads = 1;
	IOF_Write(Command_Register, ProgramCommand);
	IOF_Write(address, data);
	ioData statusReg = IOF_Read(Status_Register);
	while (!TestByteSet(statusReg,7))
	{
		statusReg = IOF_Read(Status_Register);
		nReads++;
		if (nReads == 10)
			return FD_TIMEOUT;
	}
	if (TestStatusRegError(statusReg, 3))
		return FD_VPP_ERROR;
	if (TestStatusRegError(statusReg, 4))
		return FD_PROGRAM_ERROR;
	if (TestStatusRegError(statusReg, 5))
		return FD_PROTECTED_WRITE_ERROR;
	if (IOF_Read(address) != data)
		return FD_READBACK_ERROR;
	return FD_SUCCESS;
}

static bool TestStatusRegError(ioData statusReg, int error)
{
	bool ret = false;
	if (TestByteSet(statusReg, error))
	{
		IOF_Write(Command_Register, ResetCommand);
		ret = true;
	}
	return ret;
}

static bool TestByteSet(uint32_t reg, int byte)
{
	return (reg & (1 << byte));
}