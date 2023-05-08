#include "gtest/gtest.h"
#include "fff.h"

extern "C" {
#include "FlashDriver.h"
#include "IO_Flash.h"
}
static void setReadOkReturns(ioData data);
static void assertWrite(int nCall, ioAddress address, ioData data);

DEFINE_FFF_GLOBALS
FAKE_VOID_FUNC(IOF_Write, ioAddress, ioData);
FAKE_VALUE_FUNC(ioData, IOF_Read, ioAddress);

class FlashDriverTest : public testing::Test
{
	void SetUp()
	{
		RESET_FAKE(IOF_Write);
		RESET_FAKE(IOF_Read);
		FFF_RESET_HISTORY();
	}

	void TearDown()
	{
	}
};

TEST_F(FlashDriverTest, FlashWritesOk)
{
	uint16_t address = 0xBEFF;
	uint8_t data = 0xAA;
	setReadOkReturns(data);

	int ret = FD_Program(address, data);

	assertWrite(1, 0x0, 0x40);
	assertWrite(2, address, data);
	ASSERT_EQ(IOF_Read_fake.return_val_history[0], 1 << 7);
	ASSERT_EQ(2, IOF_Write_fake.call_count);
	ASSERT_EQ(ret, FD_SUCCESS);
}

static void setReadOkReturns(ioData data)
{
	ioData oKReturns[5] = { 1 << 7, data, 0, 0, 0 };
	SET_RETURN_SEQ(IOF_Read, oKReturns, 3);
}

static void assertWrite(int nCall, ioAddress address, ioData data)
{
	ASSERT_EQ(IOF_Write_fake.arg0_history[nCall - 1], address);
	ASSERT_EQ(IOF_Write_fake.arg1_history[nCall - 1], data);
}