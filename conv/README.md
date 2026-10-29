# The conventions suite

Copyright (c) 2026 Nicholas Martin. All rights reserved.

A third suite beside the POSIX.1-2024 tests (`modules/`) and the System V
ABI tests. It measures how well a system follows specifications that are
not POSIX, and practices that no one has written into a standard. It reuses
the driver (`posixcheck.sh`), the result codes and the reporting; only the
module tree is separate.

    sh conv/run.sh [driver options] [conv/xdg ...]     the suite
    sh conv/neg/run.sh                                  negative checks
    python3 ci/lint.py --modules conv/modules           traceability lint

`conv/run.sh` is `posixcheck.sh --root conv/modules` plus two copies of the
caller's environment (below). All driver options work (`--results DIR`,
`--only CONV.tzif.003`, `--no-selftest`, `--mutant NAME`).

## Two tiers, reported separately

| Tier       | What backs it                              | Assertion class | A failure is | Reported |
|------------|--------------------------------------------|-----------------|--------------|----------|
| FORMAL     | a published specification (an RFC)         | `required`      | a conformance bug | `FAIL` |
| CONVENTION | common practice, or a specification that is an agreement among projects (XDG, the layout descriptions, the BSD file-format pages) | `disputed` | a deviation for a person to judge | `INSPECT` |

The driver's class `disputed` turns a failing result into `INSPECT`, which
is the right generic mechanism: no change to the driver's logic was needed.
Every assertion's `note` line states its tier, the source, and whether the
sources disagree. Where sources disagree the check accepts each variant or
does not assert (the notes say which). A FORMAL assertion that only repeats
a SHOULD (CONV.tzif.004) is `disputed`, since a SHOULD is not a conformance
bug.

## Areas (first batch)

| Module        | Tier        | Assertions | Source |
|---------------|-------------|-----------:|--------|
| `conv/xdg`      | CONVENTION | 4  | XDG Base Directory Specification 0.8 |
| `conv/layout`   | CONVENTION | 10 | Filesystem Hierarchy Standard 3.0; the hier(7) manuals of the three BSD projects |
| `conv/devnodes` | CONVENTION | 7  | the same, plus the FHS operating-system annex |
| `conv/etcdb`    | CONVENTION | 15 | passwd(5), group(5), hosts(5), services(5), protocols(5), shells(5), resolv.conf(5) of the BSD projects; IANA protocol numbers |
| `conv/tzif`     | FORMAL     | 5  | RFC 8536 |
| `conv/utf8`     | FORMAL     | 8  | RFC 3629 |

Assertion ids are `CONV.<area>.NNN` and the driver maps them to
`conv/modules/conv/<area>/`. Tests are C17 or POSIX sh, ephemeral, and give
the same verdict as root and as an ordinary user. No test, assertion or
note names an operating system or a C library (`ci/lint.py` checks this for
the conv tree); records of one system's observations go in
`known-deviations.md` and `STATUS.md`.

## Environment

The driver gives a test a clean environment and passes only `PC_*`
variables. These overrides are therefore read by the tests
(`PC_CONV_<NAME>`); `conv/run.sh` fills two from the caller's environment.

| Variable | Meaning |
|----------|---------|
| `PC_CONV_XDG_RUNTIME_DIR` | the XDG_RUNTIME_DIR to judge (copied from `XDG_RUNTIME_DIR`); unset gives NOTINUSE |
| `PC_CONV_TZDIR` | the zone database to judge (copied from `TZDIR`); else the common locations are tried |
| `PC_CONV_ROOT` | prefix for the paths of conv/layout and conv/xdg.004 (fixtures) |
| `PC_CONV_DEV` | directory used instead of /dev (fixtures) |
| `PC_CONV_PTY_PREFIX` | directory the pty slave name must begin with (default /dev/) |
| `PC_CONV_PASSWD`, `_GROUP`, `_SHELLS`, `_HOSTS`, `_SERVICES`, `_PROTOCOLS`, `_RESOLV` | file read instead of the /etc file (fixtures) |

A test whose file does not exist is NOTINUSE (another database may be in
use). Checks that compare the C library with a file are NOTINUSE when the
file is overridden, because the library reads the real one.

## Negative checks

`conv/neg/run.sh` builds fixtures in a private directory and shows each area's
checks failing: a runtime directory of mode 0755, a `/tmp` without the sticky
bit, a passwd line with five fields, TZif files with unsorted transitions and
other planted faults (built by `conv/neg/mktz.c`), and planted faults in
`mbrtowc`, `wcrtomb`, `link`, `mkdir` and `localtime` through the driver's
mutant mechanism (`conv/modules/xsh/<function>/mutants/`, also usable with
`ci/mutate.sh --root conv/modules 'CONV.*'`). Controls with correct fixtures
show the fixtures themselves are not what fails. Cases where the system under
test already fails natively (a baseline FAIL) cannot be judged by
`ci/mutate.sh`; it reports them UNCHECKED, and `conv/neg/run.sh` reports them
by their native result.

## Not assertable (and why)

- **XDG relative paths** ("a relative path in an XDG variable is invalid and
  must be ignored"), variable precedence, search order, `~/.local/bin` in
  `PATH`: duties of an application or of the distribution's session set-up.
  No tool that every system has implements the lookup (checked: the
  desktop-integration tools that may exist are third-party and not part of a
  base system). A test would have to implement the rule itself and so only
  test itself. Not asserted.
- **XDG_RUNTIME_DIR lifetime** (created at first login, removed at last logout,
  gone after reboot), "local file system", file-change notifications and the
  periodic clean-up rules: not observable by a program through interfaces every
  system has. CONV.xdg.002 tests the features that are.
- **Layout**: `/home`, `/root`, `/opt`, `/srv`, `/boot`, `/media`, `/run`, `/lib`
  (only one family of sources lists them, or the sources disagree about merged
  trees). The mode of `/tmp` and `/var/tmp` is stated by none of the five
  sources: world access follows from "available to programs", the sticky bit
  is practice that no listed source records (marked so).
- **Device nodes**: `/dev/full` (no source lists it); `/dev/console` and the
  exact name of the pty master (POSIX or no source); existence of
  `/dev/random` and `/dev/urandom` (only one source lists one); pty slave mode
  and owner; device numbers (FHS says there is no requirement).
- **etcdb**: the contents of any file beyond its line format; password hashes,
  the gecos field, login-name character sets beyond "no white space, no
  leading hyphen" (the pages differ), line-length limits, name-service order.
  The manual pages of other families of systems could not be reached from the
  build environment (that host is blocked), so only what the BSD pages share
  is asserted.
- **tzif**: correctness of the zone data; instants after the last transition
  (other than the footer's agreement with the last transition); TZ-string
  extensions are not evaluated through the system. **utf8**: byte order mark,
  noncharacters, normalization.
- **Deferred to later batches**: terminfo; ECMA-48 and xterm control sequences
  through a pty; loopback TCP and UDP and DNS behaviour from the RFCs; syslog
  message format (RFC 5424); ELF loader conventions; a common library
  extensions profile.

## Layout of the tree

    conv/run.sh               the wrapper
    conv/lib/                 cv.h (helpers), tzif.h (TZif reader), u8.h (UTF-8 reference)
    conv/modules/conv/<area>/ MODULE, <area>.asr, t/NNN-name.{c,sh}
    conv/modules/xsh/<fn>/mutants/   planted faults for the negative checks
    conv/neg/                 negative checks and the TZif fixture writer
    conv/known-deviations.md  records of one system's observations
    conv/STATUS.md            running log
