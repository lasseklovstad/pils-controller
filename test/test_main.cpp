#include <unity.h>
#include "Controller.h"
#include "Controller.cpp"
#include <ArduinoFake.h>

void test_example(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void test_happy_case_warm_active_shouldTurnOnSource(void){
    Controller controller(1, "secret");
    controller.update("warm", "active", "1000-18.0;2000-15.0;4000-25.3");
    controller.setTemperature(18.2);

    TEST_ASSERT_EQUAL(controller.getMode(), Mode::WARM);

    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), false);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), false);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), false);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), true);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), true);
}

void test_happy_case_cold_active_shouldTurnOnSource(void){
    Controller controller(1, "secret");
    controller.update("cold", "active", "1000-18.0;2000-15.0;4000-25.3");
    controller.setTemperature(18.2);

    TEST_ASSERT_EQUAL(controller.getMode(), Mode::COLD);

    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), true);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), true);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), true);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), false);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), false);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_example);
    RUN_TEST(test_happy_case_warm_active_shouldTurnOnSource);
    RUN_TEST(test_happy_case_cold_active_shouldTurnOnSource);
    UNITY_END();
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_example);
  RUN_TEST(test_happy_case_warm_active_shouldTurnOnSource);
  RUN_TEST(test_happy_case_cold_active_shouldTurnOnSource);
  return UNITY_END();
}

int main(void) {
  return runUnityTests();
}