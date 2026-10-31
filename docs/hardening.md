# Hardening: the suite's own test suite

Copyright (c) 2026 Nicholas Martin. All rights reserved.

A conformance suite is only worth its verdicts if the tests are shown, by
running them, to be neither too lax nor too strict. Each assertion therefore
gets two kinds of evidence, produced against the standard's text and checked
by `ci/mutate.sh`:

- **Faults (MUTANT).** A plausible wrong implementation that breaks exactly
  the requirement the assertion states. The test must FAIL. A fault the test
  passes (MISSED) means the test is too lax.
- **Variants (VARIANT).** A conforming implementation that makes a different
  choice where the text leaves one open. The test must PASS. A variant it
  fails (REJECTED) means the test asks for more than the text.

`docs/variants.md` describes the mechanics. The rest of this file is how the
work is done and what the first round taught.

## Procedure for one module

1. Read the assertions (`<module>.asr`) and the standard's page. List, for
   each requirement, the typical mistakes and the choices left open.
2. Write the faults and variants; declare them in each test's header.
3. Run `sh ci/mutate.sh 'ID-pattern'` for the module. Every fault CAUGHT, no
   variant REJECTED, nothing FLAKY. A fault that is MISSED is fixed in the
   test, never by weakening the fault. A variant that is REJECTED is fixed in
   the test, never by deleting the variant.
4. Check each fault against the text again: a fault the text permits is a bug
   in the fault.
5. An independent reviewer, given only the standard and the code, tries to
   show that a fault is allowed, that a strengthened test asks for too much,
   or that a variant is missing.
6. A fault that cannot be judged on the machine (the baseline does not PASS
   there) is reported UNCHECKED and run where it can be. It is not counted as
   caught.

## Patterns that worked

- **Utilities.** One small reimplementation in `mutants/` with a switch for the
  fault (`sort`, `cut`), or a wrapper that runs the real utility and alters
  one thing (`head`). Built-ins (`read`) are shadowed by shell functions that
  reach the real one with `command`.
- **Functions.** A preload library whose functions call the real ones and
  alter one thing.
- **Locale-dependent faults** are wrappers around the environment variables;
  they can only be judged where a locale made by `localedef` can be selected.

## What the first round found (cut, head, read, rename, sort)

About 270 faults and 22 variants over 5 modules and 89 assertions. Findings:

- Tests were too lax in several places (a missing diagnostic on standard
  error, a header not checked, a large count or binary data not exercised);
  about 25 tests were strengthened, each against the text.
- A helper that turns "the expected file is missing" into UNRESOLVED hides
  faults that delete or fail to create it. Tests now check existence first.
  Other modules probably have the same pattern; a sweep is queued.
- A baseline that already fails on the machine leaves its faults UNCHECKED.
  Faults for those assertions must be checked on a system where the baseline
  passes.
- A variant that passes on its first run proves nothing until it has been
  seen to take effect; check that it is actually planted.
- `ci/mutate.sh` cannot choose the shell under test; a `--sut-sh` option is
  queued so one module can be checked under several shells.

## Held and queued

- `strtol` is hardened (56 faults, all caught) but not merged: its fault
  library names symbols that one C library uses for redirection, and nothing
  in the repository may name a library. The same is true of the existing
  `fscanf`, `vfscanf` and `strerror` fault libraries. The remedy is a build
  step that compiles a probe against the same headers, reads the symbol names
  it references, and passes them to the fault library as macros, with a
  fixture in `ci/selftest-mutate.sh`.
- One dedicated assertion in XBD 8.2 for "a pathname in a locale variable
  selects the localedef output", so the many tests that only use such a
  locale can stay UNRESOLVED when it is unsupported.

## Backlog

`python3 ci/lint.py` reports every assertion that declares neither a MUTANT
nor `nomutant`: 6,373 at the time of writing, in 772 modules (functions about
3,970, utilities about 2,110, headers about 290). The order of work is by
risk: modules whose tests have already produced test bugs first, then the
widely used functions and utilities, then the rest. Work in batches of three
or four workers, one module each, each in its own worktree, with the review
step above before merging.
