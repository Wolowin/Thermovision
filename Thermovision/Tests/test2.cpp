#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "boost/shared_ptr.hpp"
#include "Interface/lut_table.h"



//TEST(DISABLED_Temp_calculator, does_return_simple_value)
//{
//	QString test;
//	LUT_table the_table;
//	XML_handler the_xml_handler;
//	the_table = the_xml_handler.get_profile_LUT_table(test);

//	EXPECT_EQ(200, the_calculator.get_temp_from_double(0));
//	EXPECT_EQ(300, the_calculator.get_temp_from_double(20));
//	map <int, double> map_to_initialize;

//	map_to_initialize[50] = 50;
//	map_to_initialize[100] = 150;
//	map_to_initialize[150] = 100;
//	map_to_initialize[200] = 200;
//	map_to_initialize[250] = 100;
//	map_to_initialize[300] = 50;
//	map_to_initialize[350] = 150;
//	map_to_initialize[400] = 250;
//	map_to_initialize[450] = 350;

//	Temp_calculator the_calculator(map_to_initialize);

//	EXPECT_EQ(425, the_calculator.get_temp_from_double(300));
//	EXPECT_EQ(400, the_calculator.get_temp_from_double(250));
//	EXPECT_EQ(388, the_calculator.get_temp_from_double(225));  // rounded up value in between
//	EXPECT_EQ(387, the_calculator.get_temp_from_double(224.5));  // rounded down
//	EXPECT_EQ(275, the_calculator.get_temp_from_double(175));  // rounded down
//	EXPECT_EQ(275, the_calculator.get_temp_from_double(175));  // checking if map works (only to be checked by log unless refactored
//	EXPECT_EQ(200, the_calculator.get_temp_from_double(125));  // checking if map works (only to be checked by log unless refactored
//	EXPECT_EQ(125, the_calculator.get_temp_from_double(75));
//	EXPECT_EQ(50, the_calculator.get_temp_from_double(25));  // under min value
//	EXPECT_EQ(450, the_calculator.get_temp_from_double(600));  // over max value
//}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
