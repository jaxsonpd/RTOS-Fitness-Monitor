/**
 * @file OrbitOLEDInterface_mock.h
 * @author Jack Duignan (Jdu80@uclive.ac.nz)
 * @date 2024-08-11
 * @brief Mock for the orbit oled module to allow for unit testing
 */

#include <stdint.h>
#include <stdbool.h>

#include "fff.h"

#include "OrbitOLED/OrbitOLEDInterface.h"

#ifdef FFF_MOCK_IMPL
#define VOID_FUNC FAKE_VOID_FUNC
#define VALUE_FUNC FAKE_VALUE_FUNC
#else
#define VOID_FUNC DECLARE_FAKE_VOID_FUNC
#define VALUE_FUNC DECLARE_FAKE_VALUE_FUNC
#endif

#define FFF_ORBIT_OLED_INPUT_FAKES_LIST(FUNC) \
    FUNC(OLEDInitialise)                        \
    FUNC(OLEDStringDraw)

VOID_FUNC(OLEDInitialise);
VOID_FUNC(OLEDStringDraw, const char*, uint32_t, uint32_t);