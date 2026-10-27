# How modules are built and checked

Copyright (c) 2026 Nicholas Martin. All rights reserved.

The aim is that every verdict is what the text of POSIX.1-2024 requires,
on any system. The rules are in RULES.md (8 to 12); this is the procedure
that applies them. It was set after an audit of how the first 1,044
assertions were built (October 2026): most mistakes were caught late, after
tests, faults and runs had been written; the authors and reviewers are the
same kind of model and tend to make the same mistakes; and the checks that
do not depend on anyone's reading are the empirical ones.

## Gates, per module

1. **Assertions before code.** The builder writes the assertions (`.asr`),
   the coverage accounting (`.cov`) and, for each assertion, a prediction:
   the result a conforming system gives, the passage it rests on, and every
   choice the text leaves open that the test must tolerate. No test code yet.
2. **Blind second reading.** A reviewer who has not seen the builder's
   predictions reads the same passages and writes its own expected results
   and list of open choices. The two are compared; each disagreement is
   settled against the text, or becomes a standard issue and a `disputed`
   assertion. This is the cheapest point to catch the commonest error,
   misreading the text.
3. **Tests, faults, variants.** Only then are the tests written, each with
   planted faults it must catch and conforming variants it must tolerate
   (docs/variants.md).
4. **Empirical gate.** `ci/mutate.sh` on every reference library: every
   fault CAUGHT, no variant REJECTED, nothing FLAKY. Then the reference
   runs, compared with the predictions; matches are reviewed as well as
   mismatches. A Linux result never changes an expectation.
5. **Adversarial review.** A reviewer given only the standard and the code
   (not the builder's reasoning or reports) tries to show that some test
   asks for more than the text, accepts less, or that a variant models a
   system the text forbids. Every finding names its passage and, where it
   can, a way to show it by running something. Agreement among reviewers
   is not proof; the empirical checks and, for `disputed` decisions, the
   owner's own review are.

## Rules for the coordinator

- Review findings are passed on verbatim, not paraphrased; follow-up
  instructions the coordinator writes go through the same review as the
  work they change.
- Tooling (`posixcheck.sh`, `ci/mutate.sh`, `ci/lint.py`) changes only with
  its self-tests passing: the driver's harness self-tests, and
  `ci/selftest-mutate.sh` (a fixture with a known answer for every outcome).
- A script that may be running is replaced by writing a new file and
  moving it into place, never edited in place.
- Speed changes (`--cache`, `-j`) are adopted only after a full run of
  both paths on the same frozen tree gives identical verdicts.
- Timings are measured more than once, on a quiet machine, with the load
  recorded.

## Calls that change the process IDs

`setuid`, `setgid`, `seteuid`, `setegid`, `setreuid` and `setregid` carry
`nomutant` on every assertion. A fault library for them would have to wrap
a call that really changes the IDs of the process that runs it, which is
not hosted by the harness. The tests are instead reviewed by hand against
the text: a reviewer who has not seen the builder's work reads each test
and says which wrong implementation it would accept. The tests probe
whether the state they need exists and are UNRESOLVED otherwise. This is a
weaker check than the empirical gate; do not extend it to other modules.

## Flaky results

An anomaly (a fault MISSED, a variant REJECTED or UNDECIDED) is never
reported on one run: `ci/mutate.sh` reruns it, five runs in all, and a
baseline that did not pass is run three times. Runs that disagree give
FLAKY, which has no verdict and fails the check until the test is fixed.

## Operating assumptions

- The suite runs as root or the equivalent. Permission errors are tested by dropping privilege in a child process. Cases for ordinary-user runs are added only where the standard strictly requires them.
- A test decides PASS or FAIL against the standard alone and never depends on which system it runs on.
- Tests keep to their private working directory; the driver removes its own directory when the run ends, so repeated runs leave nothing behind.
