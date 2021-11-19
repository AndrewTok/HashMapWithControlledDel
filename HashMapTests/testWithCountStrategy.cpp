#include "pch.h"
#include "HashMapControllDel.h"
#include "CountStrategy.h"


//class TestHashMap : public ::testing::Test
//{
//protected:
//	void SetUp()
//	{
//		test_data data;
//		CountStrategy strategy()
//		chipher = new XorChipher(BUFF_SIZE, data.key);
//
//	}
//	void TearDown()
//	{
//		delete chipher;
//	}
//	void encode_decode()
//	{
//		data.pop_num = chipher->pop(data.out_data, BUFF_SIZE);
//		ASSERT_EQ(data.pop_num, BUFF_SIZE);
//		data.push_num = chipher->push(data.out_data, BUFF_SIZE);
//		ASSERT_EQ(data.push_num, BUFF_SIZE);
//		data.pop_num = chipher->pop(data.out_data, BUFF_SIZE);
//		ASSERT_EQ(data.pop_num, BUFF_SIZE);
//		ASSERT_STREQ(data.out_data, data.test_input_data);
//	}
//	test_data data;
//	XorChipher* chipher;
//};
//TEST(TestInsertW)