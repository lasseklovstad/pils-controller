#include <unity.h>

void test_example(void) {
    TEST_ASSERT_EQUAL(1, 1);
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

// WARNING!!! PLEASE REMOVE UNNECESSARY MAIN IMPLEMENTATIONS //

/**
  * For native dev-platform or for some embedded frameworks
  */
int main(void) {
  return runUnityTests();
}