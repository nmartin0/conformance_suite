# Waivers

Copyright (c) 2026 Nicholas Martin. All rights reserved.

A waiver records a known FAIL that should not count against a verdict, with
a reason and an expiry. Waivers are applied only during analysis
(`analysis/analyze.py`), never on the system under test, and the raw log
always keeps the FAIL.

Files are `*.waiver`, one waiver per line:

    assertion  system  version  expires     reason...
    XSH.open.011  linux-*  *  2027-06-30  glibc has no O_CLOFORK yet; tracked upstream

`system` and `version` are shell-style patterns matched against the run's
name and release. An expired waiver stops matching.
