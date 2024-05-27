#include "testGenericGpio.h"


ZTEST_SUITE(testGenericGpio, NULL, NULL, NULL, NULL, NULL);

ZTEST(testGenericGpio, test_Switch_Config_one)
{
	bool a =  switchesConfig();
	zassert_equal(a,0);
}

ZTEST(testGenericGpio, test_Switch_Init_one)
{	
	int8_t a = switchesInit();
	zassert_equal(a,0);
}

ZTEST(testGenericGpio, test_Switch_Get_one)
{

	for(uint8_t i = 0; i < 5; i++)
	{
		uint8_t a = switchesGet(i);
		zassert_equal(a,0);
	}
}


ZTEST(testGenericGpio, test_abc_Config_one)
{
	bool a = abcbuttonsConfig();
	zassert_equal(a,0);
	bool a = abcledsConfig();
	zassert_equal(a,0);
}

ZTEST(testGenericGpio, test_abc_Init_one)
{	
	uint8_t a = abcbuttonsInit();
	zassert_equal(a,0);
	uint8_t a = abcledsInit();
	zassert_equal(a,0);
}

ZTEST(testGenericGpio, test_abc_GetSet_one)
{

	uint8_t a = abcbuttonsGet('a');
	zassert_equal(a,0);
	uint8_t a = abcledsSet('a',0);
	zassert_equal(a,0);

	uint8_t a = abcbuttonsGet('b');
	zassert_equal(a,0);
	uint8_t a = abcledsSet('b',0);
	zassert_equal(a,0);

	uint8_t a = abcbuttonsGet('c');
	zassert_equal(a,0);
	uint8_t a = abcledsSet('c',0);
	zassert_equal(a,0);
}



ZTEST(testGenericGpio, test_start_Config_one)
{
	bool a = startbuttonledConfig();
	zassert_equal(a,0);
}

ZTEST(testGenericGpio, test_start_Init_one)
{	
	uint8_t a = startbuttonledInit();
	zassert_equal(a,0);

}

ZTEST(testGenericGpio, test_start_GetSet_one)
{
	uint8_t a = startbuttonGet();
	zassert_equal(a,0);
	uint8_t a = startledSet(0);
	zassert_equal(a,0);
}