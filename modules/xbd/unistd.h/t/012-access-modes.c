/*
 * The access() mode constants and their combinations differ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.012
 */
#include <unistd.h>
#include "pc.h"

#if F_OK == R_OK
#error "F_OK and R_OK have the same value"
#endif
#if F_OK == W_OK
#error "F_OK and W_OK have the same value"
#endif
#if F_OK == X_OK
#error "F_OK and X_OK have the same value"
#endif
#if F_OK == (R_OK | W_OK)
#error "F_OK and (R_OK | W_OK) have the same value"
#endif
#if F_OK == (R_OK | X_OK)
#error "F_OK and (R_OK | X_OK) have the same value"
#endif
#if F_OK == (R_OK | W_OK | X_OK)
#error "F_OK and (R_OK | W_OK | X_OK) have the same value"
#endif
#if R_OK == W_OK
#error "R_OK and W_OK have the same value"
#endif
#if R_OK == X_OK
#error "R_OK and X_OK have the same value"
#endif
#if R_OK == (R_OK | W_OK)
#error "R_OK and (R_OK | W_OK) have the same value"
#endif
#if R_OK == (R_OK | X_OK)
#error "R_OK and (R_OK | X_OK) have the same value"
#endif
#if R_OK == (R_OK | W_OK | X_OK)
#error "R_OK and (R_OK | W_OK | X_OK) have the same value"
#endif
#if W_OK == X_OK
#error "W_OK and X_OK have the same value"
#endif
#if W_OK == (R_OK | W_OK)
#error "W_OK and (R_OK | W_OK) have the same value"
#endif
#if W_OK == (R_OK | X_OK)
#error "W_OK and (R_OK | X_OK) have the same value"
#endif
#if W_OK == (R_OK | W_OK | X_OK)
#error "W_OK and (R_OK | W_OK | X_OK) have the same value"
#endif
#if X_OK == (R_OK | W_OK)
#error "X_OK and (R_OK | W_OK) have the same value"
#endif
#if X_OK == (R_OK | X_OK)
#error "X_OK and (R_OK | X_OK) have the same value"
#endif
#if X_OK == (R_OK | W_OK | X_OK)
#error "X_OK and (R_OK | W_OK | X_OK) have the same value"
#endif
#if (R_OK | W_OK) == (R_OK | X_OK)
#error "(R_OK | W_OK) and (R_OK | X_OK) have the same value"
#endif
#if (R_OK | W_OK) == (R_OK | W_OK | X_OK)
#error "(R_OK | W_OK) and (R_OK | W_OK | X_OK) have the same value"
#endif
#if (R_OK | X_OK) == (R_OK | W_OK | X_OK)
#error "(R_OK | X_OK) and (R_OK | W_OK | X_OK) have the same value"
#endif

int main(void)
{
	pc_begin("XBD.unistd.h.012");
	pc_test();
	pc_pass();
}
