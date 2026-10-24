/*
 * sig2str.rtmax_upper.h - conforming variant (CI only): sig2str() names
 * realtime signals above the midpoint "RTMAX-m", names other valid
 * signals outside the table by their decimal number, and str2sig() also
 * takes names with the SIG prefix; SIG2STR_MAX is the least size that
 * holds every name (9: "RTMIN+nn" and the null byte).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * sig2str() DESCRIPTION allows either RTMIN+n or RTMAX-m above
 * (SIGRTMIN+SIGRTMAX)/2 and any unique form for signals outside the
 * table, and str2sig() may recognise additional implementation-dependent
 * forms; XBD <signal.h> only asks SIG2STR_MAX to cover the longest name.
 */
#define PCV_RTMAX_UPPER 1
#define PCV_SIG2STR_MAX 9
#include "sig2str-impl.inc"
