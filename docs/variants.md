# Conforming variants

Copyright (c) 2026 Nicholas Martin. All rights reserved.

A planted fault (MUTANT) shows that a test is not too lax: the fault breaks
a requirement and the test must FAIL. A conforming variant (VARIANT) shows
that a test is not too strict: the variant does something the standard
allows but the systems we try the suite on may not do, and the test must
still PASS. Together they check a test against the text itself, without
trusting any one system's behaviour (RULES.md 8 to 11).

## When a test needs one

Wherever the text leaves the implementation a choice that the test's
outcome could depend on. The usual kinds:

- **errors**: XSH 2.3 lets an implementation report any of several
  applicable errors, and "may fail" errors may be reported or not. A
  variant returns the other permitted error, or the may-fail error where
  it applies.
- **values**: a symbolic constant with another permitted value (UTIME_NOW
  as -1), an implementation-defined limit at its minimum, an unspecified
  value (a pointer, an ordering, a padding byte) set to something unusual.
- **timing**: a signal or wakeup later than usual where no bound is
  stated, a sleep that returns early only where the text allows it.
- **amounts**: short reads and writes where partial transfers are
  allowed, fewer entries per readdir() call, a buffer flushed earlier.
- **ordering**: unspecified order of directory entries, of signal
  delivery, of exit-time actions where the text leaves it open.
- **optional behaviour**: a feature the implementation may or may not
  provide, behaving the other way.

A variant must model a whole conforming system, consistently: if UTIME_NOW
is -1, then this system's real UTIME_NOW is an ordinary invalid value. A
variant that no conforming system could show is a bug in the variant.

## How

- In the test header, next to the MUTANT lines:

      * VARIANT   futimens.utime_now_m1

  The part before the dot names the directory, `modules/xsh/<prefix>/variants/`,
  exactly as for faults (`xcu/<utility>/variants/<name>.sh` for shell tests).
- A C variant is a preload library built from `variants/*.c` (include
  `pcvariant.h`; `variant_of("<prefix>.")` and `IS("name")` select it) and,
  when the variant changes what the headers define, a header
  `variants/<prefix>.<name>.h`, included before the test (the test is then
  built apart). Either or both.
- Unlike a fault, a variant applies in every phase (setup too) and every
  process, as the system's own behaviour would.
- Next to each variant, a comment says which passage allows it, in our
  own words (RULES.md 2). The commit message or review note gives the
  anchor.
- `ci/lint.py` checks that each VARIANT is implemented; `sh ci/mutate.sh
  --variants-only 'ID-pattern'` runs them and reports each one:
  - TOLERATED: the test passed. Good.
  - REJECTED: the test failed (or INSPECT): it asks for more than the
    text. Fix the test against the text. Never delete the variant because
    a reference system behaves otherwise.
  - UNDECIDED: the test could not judge (UNRESOLVED and the like). Not
    fatal, but a conforming system that behaves this way gets no verdict;
    look at whether the test can do better.
  - UNCHECKED: the baseline does not PASS here, or the variant cannot be
    planted here.

## Limits

A preload library can only change what goes through the C library's
exported functions; kernel behaviour seen through other paths (a signal's
timing, what another process sees) can be varied only as far as a wrapper
can hold it back or reshape it. Where a choice cannot be planted, the
assertion's note says so, and the test is reviewed against the text by
hand.
