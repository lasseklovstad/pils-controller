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
  controller.update("warm", "active", "1000-18.0;2000-15.0;4000-25.3", "", "", "");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::WARM);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::ACTIVE);
  TEST_ASSERT_EQUAL(controller.getMaxBufferSize(), 5);
  TEST_ASSERT_EQUAL(controller.getMinSwitchDelay(), 30000);
  TEST_ASSERT_EQUAL(controller.getHysteresis(), 0.0);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0},
                             {4000, 25.3}},
                            controller.getTemperaturePeriods());
  controller.updateSource(0);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(1500);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(2000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(4000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(5000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
}

void test_happy_case_cold_active_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("cold", "active", "1000-18.0;2000-15.0;4000-25.3", "", "", "");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::COLD);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::ACTIVE);
  TEST_ASSERT_EQUAL(controller.getMaxBufferSize(), 5);
  TEST_ASSERT_EQUAL(controller.getMinSwitchDelay(), 30000);
  TEST_ASSERT_EQUAL(controller.getHysteresis(), 0.0);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0},
                             {4000, 25.3}},
                            controller.getTemperaturePeriods());

  controller.updateSource(0);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(1500);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(2000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(4000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(5000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
}

void test_happy_case_none_inactive_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("none", "inactive", "", "", "", "");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::NONE);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::INACTIVE);
  TEST_ASSERT_EQUAL(controller.getMaxBufferSize(), 5);
  TEST_ASSERT_EQUAL(controller.getMinSwitchDelay(), 30000);
  TEST_ASSERT_EQUAL(controller.getHysteresis(), 0.0);
  compareTemperaturePeriods({}, controller.getTemperaturePeriods());

  controller.updateSource(0);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(1500);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(2000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(4000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(5000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
}

void test_happy_case_cold_prepare_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("cold", "prepare", "1000-18.0;2000-15.0", "", "", "");
  controller.setTemperature(18.2);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::COLD);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::PREPARE);
  TEST_ASSERT_EQUAL(controller.getMaxBufferSize(), 5);
  TEST_ASSERT_EQUAL(controller.getMinSwitchDelay(), 30000);
  TEST_ASSERT_EQUAL(controller.getHysteresis(), 0.0);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0}},
                            controller.getTemperaturePeriods());

  controller.updateSource(0);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(1500);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(2000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(4000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(5000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);

  controller.setTemperature(17.9);

  controller.updateSource(0);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(1500);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(2000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(4000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(5000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
}

void test_happy_case_warm_prepare_shouldTurnOnSource(void)
{
  Controller controller(1, "secret", 1);
  controller.update("warm", "prepare", "1000-18.0;2000-15.0", "", "", "");
  controller.setTemperature(17.9);

  TEST_ASSERT_EQUAL(controller.getMode(), Mode::WARM);
  TEST_ASSERT_EQUAL(controller.getStatus(), Status::PREPARE);
  TEST_ASSERT_EQUAL(controller.getMaxBufferSize(), 5);
  TEST_ASSERT_EQUAL(controller.getMinSwitchDelay(), 30000);
  TEST_ASSERT_EQUAL(controller.getHysteresis(), 0.0);
  compareTemperaturePeriods({{1000, 18.0},
                             {2000, 15.0}},
                            controller.getTemperaturePeriods());

  controller.updateSource(0);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(1500);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(2000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(4000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);
  controller.updateSource(5000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), true);

  controller.setTemperature(18.3);

  controller.updateSource(0);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(1500);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(2000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(4000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
  controller.updateSource(5000);
  TEST_ASSERT_EQUAL(controller.getIsSourceOn(), false);
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