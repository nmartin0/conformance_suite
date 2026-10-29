# Conventions suite: status

Copyright (c) 2026 Nicholas Martin. All rights reserved.

Branch `conv/unix`, worktree `/home/claude/posixcheck-conv`. Running log;
read this first when resuming.

## Source key

The BSD hier(7) and section 5 manuals are cited as BSD-A, BSD-B and BSD-C in
the assertions, in the order of the file names under `conv-docs`:
BSD-A = FreeBSD, BSD-B = NetBSD, BSD-C = OpenBSD. The FHS pages were fetched
from https://refspecs.linuxfoundation.org/FHS_3.0/fhs/ (ch03, ch04, ch05,
ch06 and the s-pages). The section 5 manual pages came from the manual-page
servers of the three projects (man.freebsd.org, man.netbsd.org,
man.openbsd.org; re-fetched in the second pass into `conv-docs/bsd5-*`).
The man7.org manual set (MAN-L in the notes) became reachable in the second
pass: passwd.5, group.5, hosts.5, services.5, protocols.5, resolv.conf.5,
shells.5, nsswitch.conf.5, hier.7, null.4 (which also covers zero), random.4,
tty.4, pts.4 and console_codes.4 are under `conv-docs/man7-*` (console_codes.4
is kept for the deferred terminal batch). Its hier(7) names FHS 3.0 as its
standard, so it is not a second witness for anything FHS states.

Toolchain source key (the keys used in `conv/modules/conv/toolchain`), fetched
into `conv-docs` with one directory per download:

| Key | Document | Host |
|-----|----------|------|
| GAS | GNU assembler manual (`binutils-as`) | sourceware.org/binutils/docs/as.html |
| GLD | GNU linker manual (`binutils-ld`) | sourceware.org/binutils/docs/ld.html |
| LLX | LLVM Extensions (`llvm-extensions`); also read: llvm-mc, CodeGenerator, LangRef (no directive reference) | llvm.org/docs |
| LLNM | llvm-nm command guide (`llvm-nm`); llvm-ar read | llvm.org/docs/CommandGuide |
| ULD | ld(1) of illumos (`illumos-ld`); ar, nm, crle also read | illumos.org/man/1/ld |
| URT | ld.so.1(1) of illumos (`illumos-ld.so.1`) | illumos.org/man/1/ld.so.1 |
| UDL | dlopen(3C), dlclose(3C), dlsym(3C) of illumos | illumos.org/man/3c |
| GABI | System V generic ABI, latest draft: ch4 (eheader, sheader, symtab, reloc), ch5 (dynamic, pheader) | sco.com/developers/gabi/latest |
| PSABI | x86-64 psABI sources, `/home/claude/abi-docs/x64` (dl.tex) | gitlab.com/x86-psABIs |

Not reachable: lld.llvm.org and clang.llvm.org (proxy refuses the host), the
illumos Linker and Libraries Guide (not at illumos.org/books; only the manual
pages are), docs.oracle.com. There is no as(1) page on illumos.org either, so
the assembler directives have one full description (GAS).

## Done (batch 1)

- Driver: `posixcheck.sh` knows the volume `conv` (two lines: ID prefix
  `CONV` to directory `conv`); nothing else changed. `ci/catalog.py` knows
  the volume; `ci/lint.py` skips the `.cov` rule and the `since` rule for it
  and refuses operating-system names in the conv tree.
- 49 assertions in 6 modules (see README). Lint clean:
  `python3 ci/lint.py --modules conv/modules` gives 0 problems (11 mutants).
- Negative checks: `sh conv/neg/run.sh`, 56 cases, 0 missed (as root and as
  `nobody`).
- `ci/mutate.sh --root conv/modules 'CONV.*'`: 11 mutants, 8 caught, 0
  missed, 3 unchecked (their baselines fail on the system: CONV.utf8.004,
  CONV.utf8.008 twice).
- POSIX suite unchanged: `./posixcheck.sh --no-selftest --only
  XBD.limits.h.017` gives the same result before and after.

## First run (root; ordinary user identical)

49 assertions: 46 PASS, 2 FAIL (CONV.utf8.004, .008), 1 INSPECT
(CONV.etcdb.013), 0 NOTINUSE with `XDG_RUNTIME_DIR` supplied (2 without).
See known-deviations.md.

## Second pass (this batch)

### Task A: man7.org cross-check of etcdb, layout and devnodes

Every etcdb, layout and devnodes assertion was re-read against the man7.org
pages. A change in an assertion is marked "code"; "note" means only the note
(and the module header) changed. Results on the reference system are the same
before and after (only CONV.etcdb.012 would be NOTINUSE if its services line
listed another source first; it does not: `db files`).

| Assertion | Pages disagree or one adds a rule | Change |
|-----------|----------------------------------|--------|
| etcdb.001 | MAN-L: seven fields (agrees); name "should not contain capital letters" (one family); silent on hyphen and white space (BSD) | note: capital letters not asserted |
| etcdb.002 | BSD: name and uid "should" be unique; MAN-L silent on uniqueness | note: sources disagree on uniqueness |
| etcdb.003 | MAN-L: empty shell means the default shell (agrees with two BSD); home is "the initial directory", not "full path" | note |
| etcdb.004 | MAN-L: four fields and a comma list; silent on spaces and uniqueness (BSD) | note |
| etcdb.005 | MAN-L nsswitch.conf: source order decides which database answers | code: NOTINUSE when the passwd or group line lists another source before the file (`compat`, `db` count as the file); `PC_CONV_NSSWITCH` for fixtures |
| etcdb.006 | MAN-L: "valid login shells", no mention of comments | note |
| etcdb.007 | MAN-L: file transfer daemons refuse shells not listed (adds a source for the use) | note |
| etcdb.008 | MAN-L restricts host-name characters (letters, digits, hyphen, period, starts with a letter); BSD does not | note: sources disagree, the wider set is accepted |
| etcdb.009 | MAN-L example calls the 127.0.0.1 and ::1 lines "desirable" | note |
| etcdb.010 | MAN-L nsswitch order for hosts | code: file-agreement part skipped when another source is listed first |
| etcdb.011 | No page (BSD or MAN-L) states a lower-case protocol name; MAN-L: protocol "should" match the protocols file (one family) | code: lower-case requirement dropped (it was asserted by no source); note |
| etcdb.012 | nsswitch order for services | code: NOTINUSE as in 005 |
| etcdb.013 | MAN-L agrees: the number is the IP header's protocol field | note |
| etcdb.014 | nsswitch order for protocols | code: `getprotobyname` comparison only when the file is first |
| etcdb.015 | MAN-L documents nameserver, search, domain (old name of search), sortlist, options; comments in the first column with # or ; | note: union of directives unchanged |
| layout (all) | MAN-L hier(7) cites FHS 3.0 as its standard: not independent | header and notes |
| layout.001, .002 | MAN-L lists the same directories; says /run now serves /var/run, which programs may still use | note |
| layout.003 | MAN-L: /tmp contents may be deleted without notice; silent on mode | note |
| layout (not asserted) | /home, /root: BSD lists both, FHS and MAN-L mark both optional: sources disagree | README and header |
| devnodes.001, .002 | MAN-L null(4) describes null, zero and tty as character files and the read and write behaviour of null and zero | note |
| devnodes.003 | MAN-L: created with mode 666, programs "act strangely" otherwise (null, zero); tty "usually" 0666 | note: now two sources for null and zero |
| devnodes.004 | MAN-L lists /dev/random and /dev/urandom; urandom does not block, /dev/random is a legacy device that may block or give a short read | code: a random device that returns EAGAIN or fewer than 32 bytes is skipped, not failed; note; README |
| devnodes.005, .006 | MAN-L hier(7) and null(4) do not list /dev/fd or the stdio names (its list is "not exhaustive") | note |
| devnodes.007 | MAN-L pts(4): slaves appear under the pts directory of the device directory; names the master (not asserted, POSIX leaves it open) | note |

Negative checks added: nsswitch order (control, NOTINUSE for 005 and 012),
the services protocol name (control and an empty name), a well-formed
protocols file.

### Task B: `conv/toolchain`, 21 assertions

CONV.toolchain.001 to .005 assembler (sections, section flags, symbol binding
and order, `.type`/`.size`, `.comm`); 006 to 017 linker (`-o`, `-l`/`-L`,
archive extraction, strong over weak, duplicates, undefined references,
common merging, `-shared`, `-soname`, `-rpath`, `-e`, `-r`); 018 to 021 loader
(`$ORIGIN`, `LD_LIBRARY_PATH`, preemption by the program, initialisation
order). Each note says which documents agree. What was dropped, with the
reason, is under "Toolchain" in README.md.

Results on the reference system (cc/as/ld/ar of the system; x86-64):
65 PASS, 2 FAIL (utf8.004, .008), 1 INSPECT (etcdb.013), 2 NOTINUSE (xdg.001,
.002 without a runtime directory), as root and as `nobody` with identical
verdicts (70 assertions in all). Other toolchains run through the checks as an
observation: the `gold` linker passes all 21; `ld.lld` with `clang` and the
LLVM assembler `llvm-mc` pass 20, with CONV.toolchain.008 INSPECT (see
known-deviations.md).

Negative checks: `sh conv/neg/run.sh` 91 cases, 0 missed, as root and as
`nobody` (35 new toolchain cases: a wrapper per fault, controls that damage
nothing, no-tool and flag-refused NOTINUSE cases). Lint:
`python3 ci/lint.py --modules conv/modules` gives 0 problems (70 assertions,
11 mutants). The toolchain tests have no library call to fault, so they are
marked `nomutant` with the reason; `ci/mutate.sh --root conv/modules 'CONV.*'`
is unchanged.

Run time of the whole conv suite: about 7 seconds (the toolchain area is most
of it: compiler and linker runs).

## Deferred (later batches)

- terminfo; ECMA-48 and xterm control sequences through the pty
  (`core/pcpty.c` exists for this)
- loopback TCP and UDP and DNS behaviour from RFC 9293, 768, 1122, 1035
- syslog message format (RFC 5424)
- a common library extensions profile
- the LLVM linker's own documentation (host unreachable); more of the second
  linker family's Linker and Libraries Guide (not at the reachable host); a
  second description of the assembler directive set (data directives, numeric
  labels) would allow those assertions
- negative checks for CONV.layout.008, CONV.devnodes.005 and CONV.etcdb.005,
  .010, .012 (they compare the system's own tools or library with the real
  files; the only way to fail them is to break the system)
- passwd/group databases that are not files (name-service back ends) are
  NOTINUSE, not judged
