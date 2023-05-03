#include "gtest/gtest.h"
#include <stdio.h>
#include <memory.h>

static char * output;

static const char* expected;

class SPrintFTest : public testing::Test
{
	void SetUp()
	{
		expected = "";
	}

	void TearDown()
	{
		free(output);
	}

};

static void expect(const char* s)
{
	expected = s;
	output = (char *)calloc(strlen(expected) + 2, sizeof(char));
	memset(output, 0xAA, strlen(expected) + 2);
}

static void given(int charsWritten)
{
	ASSERT_EQ(strlen(expected), charsWritten);
	ASSERT_STREQ(expected, output);
	ASSERT_EQ(0xaa, (uint8_t)output[strlen(expected) + 1]);
}

TEST_F(SPrintFTest, NoFormatOperations){
	expect("hey");
	given(sprintf(output, "hey"));
}

TEST_F(SPrintFTest, InsertString) {
	expect("Hello World\n");
	given(sprintf(output, "Hello %s\n", "World"));
}

TEST_F(SPrintFTest, InsertInteger) {
	expect("Number 9");
	given(sprintf(output, "Number %d", 9));
}


