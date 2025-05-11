#include <unity.h>
#include "Controller.h"
#include "Controller.cpp"
#include <ArduinoFake.h>

void test_example(void)
{
  TEST_ASSERT_EQUAL(1, 1);
}

void compareTemperaturePeriods(const std::vector<std::pair<unsigned long, float>> &expected, const std::vector<std::pair<unsigned long, float>> &actual)
{
  TEST_ASSERT_EQUAL(expected.size(), actual.size());
  for (size_t i = 0; i < expected.size(); ++i)
  {
    TEST_ASSERT_EQUAL(expected[i].first, actual[i].first);
    TEST_ASSERT_FLOAT_WITHIN(0.01, expected[i].second, actual[i].second);
  }
}

void test_happy_case_warm_active_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("warm", "active", "1000-18.0;2000-15.0;4000-25.3");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::WARM);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::ACTIVE);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0},
                             {4000, 25.3}},
                            controller.getTemperaturePeriods());

  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), true);
}

void test_happy_case_cold_active_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("cold", "active", "1000-18.0;2000-15.0;4000-25.3");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::COLD);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::ACTIVE);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0},
                             {4000, 25.3}},
                            controller.getTemperaturePeriods());

  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), false);
}

void test_happy_case_none_inactive_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("none", "inactive", "");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::NONE);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::INACTIVE);
  compareTemperaturePeriods({}, controller.getTemperaturePeriods());

  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), false);
}

void test_happy_case_cold_prepare_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("cold", "prepare", "1000-18.0;2000-15.0");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::COLD);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::PREPARE);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0}},
                            controller.getTemperaturePeriods());

  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), true);

  controller.setTemperature(17.9);

  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), false);
}

void test_happy_case_warm_prepare_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("warm", "prepare", "1000-18.0;2000-15.0");
  controller.setTemperature(17.9);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::WARM);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::PREPARE);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0}},
                            controller.getTemperaturePeriods());

  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), true);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), true);

  controller.setTemperature(18.3);

  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), false);
  TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), false);
}

void setup()
{
  UNITY_BEGIN();
  RUN_TEST(test_example);
  RUN_TEST(test_happy_case_warm_active_shouldTurnOnSource);
  RUN_TEST(test_happy_case_cold_active_shouldTurnOnSource);
  RUN_TEST(test_happy_case_warm_prepare_shouldTurnOnSource);
  RUN_TEST(test_happy_case_none_inactive_shouldTurnOnSource);
  RUN_TEST(test_happy_case_cold_prepare_shouldTurnOnSource);
  UNITY_END();
}

int runUnityTests(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_example);
  RUN_TEST(test_happy_case_warm_active_shouldTurnOnSource);
  RUN_TEST(test_happy_case_cold_active_shouldTurnOnSource);
  RUN_TEST(test_happy_case_warm_prepare_shouldTurnOnSource);
  RUN_TEST(test_happy_case_none_inactive_shouldTurnOnSource);
  RUN_TEST(test_happy_case_cold_prepare_shouldTurnOnSource);
  return UNITY_END();
}

int main(void)
{
  return runUnityTests();
}