# posixcheck

A proprietary, unofficial, clean-room conformance checker for POSIX.1-2024 (Issue 8),
written in POSIX `sh` and C.

- One driver script, `posixcheck.sh`, runs on the system under test using only
  its own `sh`, C compiler and `getconf`.
- Results go to a plain-text log (`results/run.log`), analyzed on another
  computer.
- Every test traces to a numbered assertion, and every assertion to a passage
  of the standard.

Design: https://claude.ai/artifact/BCH6E4hts9mcGb2t44GRW5

Status and next steps: see `STATUS.md`.

Not affiliated with the IEEE or The Open Group. POSIX is a trademark of the
IEEE. This project does not certify conformance.

Copyright (c) 2026 Nicholas Martin. All rights reserved. See `LICENSE`.
