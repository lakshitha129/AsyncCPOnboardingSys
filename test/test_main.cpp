#include <Arduino.h>
#include <unity.h>
#include "AsyncCPOnboardingSys.h"

AsyncCPOnboardingSys portal;

void setUp(void) {
    // Setup code before each test
}

void tearDown(void) {
    // Cleanup code after each test
}

void test_portal_initialization(void) {
    TEST_ASSERT_FALSE(portal.isPortalActive());
}

void test_portal_activation(void) {
    portal.begin("TEST-AP");
    TEST_ASSERT_TRUE(portal.isPortalActive());
}

void setup() {
    delay(2000);
    UNITY_BEGIN();
    
    RUN_TEST(test_portal_initialization);
    RUN_TEST(test_portal_activation);
    
    UNITY_END();
}

void loop() {
    // Empty
}