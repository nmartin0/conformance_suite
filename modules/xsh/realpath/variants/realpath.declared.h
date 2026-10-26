/*
 * realpath.declared.h - conforming variant: <stdlib.h> declares realpath()
 * (CI only; a header alone).
 *
 * posixcheck: shim (a build shim: supplies declarations only; ci/mutate.sh
 * reports a failing run under it as SYSTEM, a verdict on the system).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * Since Issue 8 realpath() is in the Base, outside the XSI option, so
 * <stdlib.h> must declare it in a strict POSIX build; this header gives a
 * conforming system's declaration where the system's own header leaves it
 * out, so the behaviour can still be checked (and faults planted).
 */
#include <stdlib.h>
char *realpath(const char *restrict file_name, char *restrict resolved_name);
