/*
 * sig2str.rtmin_all.h - conforming variant (CI only): sig2str() names
 * every realtime signal between SIGRTMIN and SIGRTMAX "RTMIN+n", names
 * other valid signals outside the table by their decimal number,
 * str2sig() takes nothing beyond the forms DESCRIPTION lists, and
 * SIG2STR_MAX is generous (64).
 *
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * sig2str() DESCRIPTION allows RTMIN+n for the whole range below
 * SIGRTMAX, and str2sig() need not recognise any extra form.
 */
#define PCV_RTMAX_UPPER 0
#define PCV_SIG2STR_MAX 64
#include "sig2str-impl.inc"
