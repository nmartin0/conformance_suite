# posixcheck

Clean-room POSIX.1-2024 conformance suite: C17 and POSIX sh. No make.

Rules that always apply:

- The standard's text is the only oracle. No test, assertion or note names
  or depends on any operating system or libc. Never copy spec text into the
  repository, and never read other test suites.
- Keep it vanilla: POSIX sh and C; Python only for development tools.
- Tests run as root and as an ordinary user and must be correct for both.
- Process and gates: `docs/methodology.md` (see "Working practice").
- Open standard questions: `docs/standard-issues.md`. Plan: `docs/roadmap.md`.

Useful commands (run from the repository root):

- `./posixcheck.sh --results DIR [--resume]` full run; `--show CODES` and
  `--failures FILE` choose what is printed and saved.
- `ci/mutate.sh` empirical gate; `ci/coverage.py` ledger; `ci/nullsys.sh`
  negative control.

Commits are small, one concern each, with the evidence in the message.
