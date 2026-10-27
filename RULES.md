# Rules

Copyright (c) 2026 Nicholas Martin. All rights reserved.

posixcheck is for personal use: verifying an operating system its owner is
developing. These rules keep it within the terms of the standard and of
every other project it learns from. They are not optional.

## The standard

1. Read the standard only from a copy obtained under The Open Group's
   terms: the free online edition, or the personal-use download
   (`susv5.tar`). Keep that copy outside this repository.
2. Never copy the standard's text into this repository: not into
   assertions, tests, comments or documentation. Assertions are written in
   our own words, with an anchor (`page#SECTION/item`) pointing at the
   passage. Names and values of interfaces (functions, macros, errno
   names, option codes) are facts and may appear.
3. Anything generated from the standard (such as the planned spec model)
   is built on the user's own machine from the user's own copy, kept
   under `build/` or `results/`, and never committed or bundled.
4. Use only published editions and interpretations from the public
   Austin Group defect tracker. Do not use material that is available only
   to members (drafts, member-only discussions).

## Other test suites

5. Never read or copy the source code of another test suite (os-test,
   the Open POSIX Test Suite, LTP, the BSD test suites, pjdfstest, The Open
   Group's suites). Their published results are not used as evidence for
   an expectation either (RULES 9). Design notes and papers are fine;
   ideas are not copyrighted, code is.
6. Do not use The Open Group's test suites at all: their licenses do not
   cover this project.

## Names

7. The project is not to be published, sold or described as certifying
   anything. "POSIX" is a trademark of the IEEE and "UNIX" of The Open
   Group; if posixcheck is ever distributed, rename it and revisit these
   rules first.

## The oracle

8. The standard is the only oracle. Every expected result comes from a
   passage of the text, cited by the assertion's anchor; no expectation
   comes from what any system does. Where the text allows several
   outcomes (XSH 2.3's choice of errors, "may fail" errors, unspecified
   and implementation-defined results, timing with no stated bound), the
   test accepts all of them. Where it leaves open something the test must
   decide, the result is UNRESOLVED, or the assertion is `disputed` with a
   standard issue; it is never what a reference system does.
9. Reference systems (Linux with glibc or musl, and later the BSDs and
   illumos) check the suite, not the other way round: that each test
   builds, runs, finishes and catches its planted faults. An expectation
   is never changed because of a run result unless the change cites the
   text; "system X does this" is not a reason. Expected non-PASS results
   on a reference system go in `known/` and `docs/linux-deviations.md`,
   as records of that system, never as the definition of correct.
10. Each assertion's expected result is predicted, with the passage it
    rests on, before its test first runs on any system; afterwards the
    matches are reviewed as well as the mismatches.
11. A test must tolerate what the text allows. Where it does allow a
    choice that the reference systems do not exercise, the test declares
    a VARIANT that plants the other choice, and `ci/mutate.sh` checks that
    the test still passes (a fault must FAIL; a variant must PASS). Each
    variant cites the passage that allows it.
12. The suite judges whatever implementation it runs on; a run describes
    that environment and nothing else. run.log records the system's name
    and the system the compiler built for, so a reader can tell, for
    example, a native FreeBSD build from a Linux build run through
    FreeBSD's Linux compatibility layer. No test or driver code branches
    on the name of an operating system; where a facility outside POSIX
    helps a test set up (setgroups()), its presence is found by building,
    not by name. (The CI libraries that plant faults and variants through
    LD_PRELOAD still name a C library's alternate symbols, such as
    glibc's open64; they decide no verdict, and go when planting moves to
    compile time.)

## Checking

13. Before each commit, check that no standard text is present:

       grep -rIl 'The IEEE and The Open Group, All Rights Reserved' . \
           --exclude-dir=.git && echo "STANDARD TEXT FOUND"

   (`ci/lint.py` does this too.)
