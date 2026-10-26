/*
 * tzset.tm_members.h - conforming variant: struct tm with the tm_gmtoff
 * and tm_zone members under their standard names (CI only; header alone).
 *
 * posixcheck: shim (a build shim: supplies declarations only; ci/mutate.sh
 * reports a failing run under it as SYSTEM, a verdict on the system).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * XBD <time.h> requires struct tm to have long tm_gmtoff and const char
 * *tm_zone. The reference systems keep them under reserved names in the
 * strict build; this header, included before the test, gives them their
 * standard names, so that a test naming them can be built and its faults
 * checked. It models a system whose <time.h> declares them as required;
 * nothing else changes. Used by tests in gmtime and localtime too.
 */
#include <time.h>
#ifndef tm_gmtoff
#define tm_gmtoff __tm_gmtoff
#endif
#ifndef tm_zone
#define tm_zone __tm_zone
#endif
