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

## Areas

| Module        | Tier        | Assertions | Source |
|---------------|-------------|-----------:|--------|
| `conv/xdg`      | CONVENTION | 4  | XDG Base Directory Specification 0.8 |
| `conv/layout`   | CONVENTION | 10 | Filesystem Hierarchy Standard 3.0; the hier(7) manuals of the three BSD projects |
| `conv/devnodes` | CONVENTION | 7  | the same, plus the FHS operating-system annex and the null, zero, random, tty and pts manuals of the man7.org set |
| `conv/etcdb`    | CONVENTION | 15 | passwd(5), group(5), hosts(5), services(5), protocols(5), shells(5), resolv.conf(5) of the BSD projects and of the man7.org manual set; nsswitch.conf(5) of the latter; IANA protocol numbers |
| `conv/tzif`     | FORMAL     | 5  | RFC 8536 |
| `conv/utf8`     | FORMAL     | 8  | RFC 3629 |
| `conv/toolchain` | CONVENTION | 21 | what two or more independent toolchain documents state alike: the GNU assembler and linker manuals, the LLVM extensions and nm documents, the second linker family's ld(1), ld.so.1(1) and dlopen(3C), the System V generic ABI and its x86-64 supplement |

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
| `PC_CONV_NSSWITCH` | name-service configuration read instead of /etc/nsswitch.conf (fixtures); a library lookup is compared with a file only when the database's line lists the file (or `compat`, or a `db` map) first, else NOTINUSE |
| `PC_CONV_CC`, `_AS`, `_LD`, `_AR` | tool used by `conv/toolchain` instead of the one `command -v` finds (`c17` then `cc`; `as`; `ld`; `ar`); a name that is not an executable file counts as absent (NOTINUSE). `conv/neg/run.sh` points these at deliberately wrong wrappers |

`conv/toolchain` tests are POSIX sh with a shared helper (`conv/lib/tc.sh`) and a small reader of the common object-file container (`conv/lib/elfdump.c`, written from the generic ABI's field layout and compiled by the test). They work in a private subdirectory that they remove, need no privilege and give the same verdict as root and as an ordinary user. The tools are found with `command -v`; a missing tool gives NOTINUSE. Compile and link options beyond the POSIX compiler utility (`-shared -fPIC -Wl,-rpath,... -Wl,-soname,...`, and a library for `dlopen`) are probed once per run; a test that needs one the compiler refuses reports NOTINUSE.

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
show the fixtures themselves are not what fails. For the toolchain area the
fixtures are tools: `conv/neg/run.sh` builds wrappers that damage one thing
(an assembler that turns `.weak` into `.globl` or drops the writable flag, a
linker that ignores `-o`, `-e`, `-soname` or `-rpath`, accepts duplicate
definitions or undefined references, extracts every archive member, accepts
an archive that comes before the object that needs it, a compiler that bakes
an absolute path in place of `$ORIGIN` or hides `LD_LIBRARY_PATH` from the
program or binds a library's calls to its own definitions) and shows each
check flag its wrapper, together with controls (wrappers that damage nothing)
and the NOTINUSE cases (no linker, no assembler, a compiler that refuses
`-fPIC`). `NEG_VERBOSE=1 sh conv/neg/run.sh` also prints each result line. Cases where the system under
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
  (the sources disagree: all three BSD manuals list `/home` and `/root`, while
  FHS and the man7.org hier(7), which names FHS as its standard and so is not a
  second witness for it, mark both optional; the others are listed by one
  family only, or the sources disagree about merged trees). The mode of `/tmp` and `/var/tmp` is stated by none of the five
  sources: world access follows from "available to programs", the sticky bit
  is practice that no listed source records (marked so).
- **Device nodes**: `/dev/full` (no source lists it); `/dev/console` and the
  exact name of the pty master (POSIX or no source); existence of
  `/dev/random` and `/dev/urandom` (one BSD manual and the man7.org manual list
  them, FHS and two BSD manuals do not: the sources disagree; the manual set
  that lists them also says `/dev/random` may block, which the test allows); pty slave mode
  and owner; device numbers (FHS says there is no requirement).
- **etcdb**: the contents of any file beyond its line format; password hashes,
  the gecos field, login-name character sets beyond "no white space, no
  leading hyphen" (the pages differ; one family adds "no capital letters" as a
  should), host-name character sets (one family restricts them, the others do
  not), a lower-case protocol name in services (no page states it), the match
  of a services protocol with the protocols file (one family, a should),
  line-length limits, the number of resolver name servers. The name-service
  order is used only to decide whether a library lookup may be compared with a
  file (CONV.etcdb.005, 010, 012, 014). Both families of pages are asserted
  only where they agree or where one is silent.
- **tzif**: correctness of the zone data; instants after the last transition
  (other than the footer's agreement with the last transition); TZ-string
  extensions are not evaluated through the system. **utf8**: byte order mark,
  noncharacters, normalization.
- **Toolchain** (the rule: only what at least two independent documents state
  alike is asserted, so each item below was checked against the documents and
  dropped for the reason given):
  - *AT&T operand order and the `%` and `$` prefixes*: described by the GNU
    assembler manual only. The LLVM documents name the syntax (a printing
    variant number) without describing it, and the x86-64 supplement only
    uses it in examples.
  - *The data directives* (`.byte .short .long .quad .ascii .asciz .string
    .align .p2align .skip`), the bytes they emit, string terminators and
    alignment padding: described by the GNU assembler manual only; the LLVM
    document covers extensions, not these. (The tests use `.byte` and `.skip`
    only to give a section content and never check the bytes.)
  - *Numeric local labels (`1:`, `1b`, `1f`)*: the GNU assembler manual only.
  - *Conventional linker symbols (`etext`, `edata`, `end`)*: the GNU linker
    manual shows them only as examples in linker scripts, and the second
    linker family's manual does not list them in the reachable pages (its
    Linker and Libraries Guide was not reachable).
  - *Order of entries inside one init array, finalisation order at exit*: the
    generic ABI only (the x86-64 supplement refers back to it). The init-array
    order between a library and its dependency is asserted (CONV.toolchain.021).
  - *`LD_PRELOAD`*: the second linker family's runtime-linker manual only.
  - *`LD_LIBRARY_PATH` before or after a recorded run path*: the generic ABI
    says before; the runtime-linker manual says only "before the default
    directories"; the GNU linker manual gives the opposite order, but for
    finding libraries at link time. That the variable adds search directories
    is asserted (CONV.toolchain.019).
  - *Constructors run by `dlopen`, destructors by `dlclose`*: the documents
    describe initialisation at program start (asserted, 021) and at exit; the
    `dlclose` manual of the second family says finalisation code runs before
    unloading, but no second document says so. Whether `dlopen` of a bare name
    consults the caller's run path: that manual says yes, the generic ABI says
    a run path serves only the dependencies recorded in the same object: the
    sources disagree.
  - *`$ORIGIN` in the argument of `dlopen`*: the generic ABI only.
  - *Symbol lookup order among a program's dependencies* (breadth first, in
    the order of the recorded dependencies) and that the recorded order is the
    command-line order: the generic ABI states the first; no second document
    states either.
  - *Whether `-L` options apply to `-l` options that precede them*: the GNU
    manual says yes, the second family's says only those that follow. The
    tests place `-L` first.
  - *The LLVM linker* (`ld.lld`): its documentation host could not be reached.
    It was run through the checks as an observation (conv/known-deviations.md).
  - *Anything about a processor other than what the directives and options
    above need*: the tests contain no instructions, and the loader tests use
    plain C.
- **Deferred to later batches**: terminfo; ECMA-48 and xterm control sequences
  through a pty (the man7.org console_codes(4) page was fetched for that
  batch); loopback TCP and UDP and DNS behaviour from the RFCs; syslog message
  format (RFC 5424); a common library extensions profile.

## Layout of the tree

    conv/run.sh               the wrapper
    conv/lib/                 cv.h (helpers), tzif.h (TZif reader), u8.h (UTF-8 reference),
                              tc.sh (toolchain test helpers), elfdump.c (object-file reader)
    conv/modules/conv/<area>/ MODULE, <area>.asr, t/NNN-name.{c,sh}
    conv/modules/xsh/<fn>/mutants/   planted faults for the negative checks
    conv/neg/                 negative checks and the TZif fixture writer
    conv/known-deviations.md  records of one system's observations
    conv/STATUS.md            running log
