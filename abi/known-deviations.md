# Known deviations and triage

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Records of what a system did, never the definition of correct. The tests do not
mention systems; this file may.

## Run 1 (batch 1, i386)

System: a 6.18 kernel on x86-64, 32-bit programs, built with gcc 13.3 `-m32`
(the default; `-O2` also tried), as root and as an unprivileged user; and once
with clang. Six consecutive runs gave identical verdicts.

    sh abi/run.sh i386 --results /tmp/abi-i386-1

| result | count |
|---|---|
| PASS | 70 |
| FAIL, INSPECT, UNRESOLVED, UNINITIATED | 0 |
| UNTESTED (untestable by design: entry.012, entry.013) | 2 |

The same run as an unprivileged user, and with `-O2`: identical. Built for
x86-64 (no `-m32`): 70 NOTINUSE, 2 UNTESTED. `abi/control.sh`: 31 detected, 0
missed (see README).

No system deviation was found with gcc. Triage of every non-PASS seen while the
batch was written:

| where | seen | decision |
|---|---|---|
| types.012 | FAIL: my expected size of `union { char; long double }` was 16 | TEST-BUG: it is 12 (size rounded to the alignment 4); fixed |
| calling.014, .015 | build failed: macro with braces and commas | TEST-BUG; fixed |
| calling.015 | FAIL on a structure holding a `long double`: the 2 padding bytes were compared | TEST-BUG: padding is undefined; that member is compared by value |
| control B, types.005 with `-malign-double` | not caught | CONTROL-DESIGN: that option does not move `long double`; `-mlong-double-64` used |
| control B, calling.020 with `-mno-fp-ret-in-387` | not caught | CONTROL-DESIGN: the option is consistent on both sides; replaced by a hand-written caller that does not pop (Part A) |
| clang: calling.016, .025 | build failed: `CMPLX` not declared | TEST-BUG: the complex values are built without it |
| clang: types.008 | FAIL: `_Alignof(max_align_t)` is less than 16 | SYSTEM deviation (the second compiler's own header); kept as a FAIL, the psABI says 16 |

No STD-AMBIG case turned up: no `disputed` assertion failed with gcc or clang.
They are disputed on the text alone (the two revisions disagree or only one
speaks); see the note of each.

## Open

- types.008 with clang on i386: `max_align_t` below 16, recorded above.
- Nothing else yet.
