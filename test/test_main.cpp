#include <unity.h>
#include "Controller.h"

void test_example(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

void test_happy_case_shouldTurnOnSource(void){
    Controller controller(1, "secret");
    controller.update("warm", "active", "1000-18.0;2000-15.0;4000-25.3");
    controller.setTemperature(18.2);

    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(0), false);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(1500), false);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(2000), false);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(4000), true);
    TEST_ASSERT_EQUAL(controller.shouldTurnOnSource(5000), true);
}

void setup() {
    UNITY_BEGIN();
    RUN_TEST(test_example);
    UNITY_END();
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_example);
  return UNITY_END();
}

int main(void) {
  return runUnityTests();
}