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
   Group's suites). Reading their published design notes, papers and
   result pages is fine; ideas are not copyrighted, code is.
6. Do not use The Open Group's test suites at all: their licenses do not
   cover this project.

## Names

7. The project is not to be published, sold or described as certifying
   anything. "POSIX" is a trademark of the IEEE and "UNIX" of The Open
   Group; if posixcheck is ever distributed, rename it and revisit these
   rules first.

## Checking

8. Before each commit, check that no standard text is present:

       grep -rIl 'The IEEE and The Open Group, All Rights Reserved' . \
           --exclude-dir=.git && echo "STANDARD TEXT FOUND"

   (`ci/lint.py` does this too.)
