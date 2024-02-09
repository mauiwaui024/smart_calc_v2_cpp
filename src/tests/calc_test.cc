#include <gtest/gtest.h>

#include "../SmartCalcV2/CalcModel.h"
#include "map"
#include "../SmartCalcV2/Token.h"

  s21::CalcModel model_;
TEST(CalcTest, Test_1) {
  model_.CalculateAnswer("2+2*2", "0");
   ASSERT_EQ(model_.GetAnswer(), 6.0);
}
TEST(CalcTest, Test_2) {
  model_.CalculateAnswer("25/37*58+14/3*2", "0");
  ASSERT_NEAR(model_.GetAnswer(), 48.522523, 1e-1);
}
TEST(CalcTest, Test_3) {
  model_.CalculateAnswer("-5.36+78/2+54-sin(1234)*cos(43)^3", "0");
   ASSERT_NEAR(model_.GetAnswer(), 87.5370351008, 1e-1);
}
TEST(CalcTest, Test_4) {
  model_.CalculateAnswer("45+sqrt(45)", "0");
   ASSERT_NEAR(model_.GetAnswer(), 51.7082039325, 1e-1);
}

TEST(CalcTest, Test_5) {
  model_.CalculateAnswer("log(983)+ln(45+34)", "0");
  // 7,36200137
    ASSERT_NEAR(model_.GetAnswer(),  7.36200137, 1e-1);
}

TEST(CalcTest, Test_6) {
  model_.CalculateAnswer("cos(23)+67^3", "0");
    ASSERT_NEAR(model_.GetAnswer(),  300762.467167, 1e-1);
}
TEST(CalcTest, Test_7) {
  model_.CalculateAnswer("cos(96)+tan(93)-ln(693.543)", "0");
    ASSERT_NEAR(model_.GetAnswer(),  -9.70962995088, 1e-1);
}
TEST(CalcTest, Test_8) {
  model_.CalculateAnswer("12365.32+(487.36/2)+98*log(90)*tan(91)", "0");
   ASSERT_NEAR(model_.GetAnswer(),  12588.6, 1e-1);
}
TEST(CalcTest, Test_9) {
  model_.CalculateAnswer("15/(7-(1+1))*3-(2+(1+1))*15/(7-(200+1))*3-(2+(1+1))*(15/(7-(1+1))*3-(2+(1+1))+15/(7-(1+1))*3-(2+(1+1)))", "0");
   ASSERT_NEAR(model_.GetAnswer(),  -30.0722, 1e-1);
}
TEST(CalcTest, Test_10) {
  model_.CalculateAnswer("5mod2", "0");
   ASSERT_NEAR(model_.GetAnswer(), 1 , 1e-1);
}

//exceptions
TEST(CalcTestException, Test_1) {
    EXPECT_THROW(model_.CalculateAnswer("2+", "0"), std::logic_error);
}
TEST(CalcTestException, Test_2) {
    EXPECT_THROW(model_.CalculateAnswer(")2+2+2+2", "0"), std::logic_error);
}
TEST(CalcTestException, Test_3) {
    EXPECT_THROW(model_.CalculateAnswer("sin((((((34)3456**(((((((((((((((#$()))))))", "0"), std::logic_error);
}
TEST(CalcTestException, Test_4) {
    EXPECT_THROW(model_.CalculateAnswer("sin5+sin5", "0"), std::logic_error);
}

TEST(CalcTestException, Test_5) {
    EXPECT_THROW(model_.CalculateAnswer("sin(54)*cos5+78*(23*)", "0"), std::logic_error);
}
TEST(CalcTestException, Test_6) {
    EXPECT_THROW(model_.CalculateAnswer("x.+334+sin(x)", "0"), std::logic_error);
}
TEST(CalcTestException, Test_7) {
    EXPECT_THROW(model_.CalculateAnswer("2 + 3 + 3)log", "0"), std::logic_error);
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
