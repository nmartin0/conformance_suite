# Triage: skips caused by option claims, absent headers and floating point (second-run system, root)

Scope: 1179 tests (866 UNSUPPORTED, 313 NOTINUSE) that the full run on the
third system skipped because an option was "not claimed", an interface was
absent, or IEC 60559 floating point was not claimed. Inputs: the system's
claim table (one line per option), the per-test outputs, `posixcheck.sh`,
`core/probe.c`, `core/options.list`, `core/pcloc.sh`, the assertion files and
the standard (XBD 1.8.1, 2.1.3 to 2.1.6, `<unistd.h>`, and the page of every
skipped interface). No standard text is copied; short phrases only.

## 1. How a claim is derived, and the check against the standard

`core/probe.c` (built with `-D_XOPEN_SOURCE=800`) reads, for every option
code in `core/options.list`, the `<unistd.h>` macro: undefined or -1 means
not claimed, above zero means claimed, zero means ask `sysconf()` (claimed
when it returns above zero). `posixcheck.sh` combines codes with `|` (either)
and `+` (both); a module whose build failure is the verdict (Tier 0) uses the
compile claim (defined and not -1). The skip happens before the test is built.

XBD `<unistd.h>` "Constants for Options and Option Groups": a symbol not
defined or defined as -1 means the option is not supported for compilation,
a value above zero means always supported at run time, zero means supported
for compilation and "might or might not" at run time. That is exactly the
probe's rule. XBD 2.1.4 ties XSI to `_XOPEN_UNIX` "a value other than -1",
which the rule also gives. The option list equals the "may support" symbols
of XBD 2.1.3.1 and 2.1.3.2 plus the robust-mutex symbols; every symbol that
Issue 8 now requires (the historical list: asynchronous I/O, barriers, clock
selection, mapped files, memory protection, monotonic clock, read-write
locks, realtime signals, semaphores, spin locks, thread-safe functions,
threads, timeouts, timers, `_POSIX2_C_BIND`) is correctly NOT in the option
list. No option the standard made mandatory is treated as optional by the
driver. Verdict on the derivation: **correct; no DERIVATION-BUG** (see 4 for
what was changed anyway).

Claim table of this system against its macro and sysconf values:

| Option | Macro | sysconf | Claim | Check |
| --- | --- | --- | --- | --- |
| ADV, CD, DC, FR, MSG, PIO, PS, RPI, RPP, SIO, SS, TPI, TPP, TPS, TSH, TSP, TYM, UU, XSI | -1 or undefined | not asked | no | follows the "-1 or undefined" rule; no macro above zero is denied |
| CPT, FSC, ML, MLR, RS, SD, SHM, SPN, TCT, TSA, TSS, UP, CRYPT | above zero | not asked | yes | follows the "above zero" rule |
| IP6 | 0 | 200112 | yes | the zero rule: sysconf above zero |

No claim is granted or denied by a compile probe that fails for a test-side
reason (the claim never compiles anything but the probe, which has every
macro in view under `_XOPEN_SOURCE=800`). The explicit `_XOPEN_UNIX` of -1 is
the system saying it is not XSI-conformant; the same system defines
`_XOPEN_CRYPT` as 1, which is its own inconsistency (the Encryption group
belongs to XSI, XBD 2.1.4.1) and only makes the 8 `XSI+CRYPT` tests of the full
run skip on XSI. The values are the previous edition's (200112, 200809), not
202405L; that is not a claim question and is already FAIL (XBD.unistd.h.004,
.006, .007, XSH.sysconf.005, .007).

## 2. Reason groups (the 1179 tests)

| Reason | Count | Class | Reasoning (standard) |
| --- | ---: | --- | --- |
| option XSI not claimed | 605 | OK, with 8 TEST-BUG and 3 STD-AMBIG (below) | `_XOPEN_UNIX` is -1, so no XSI marking binds (XBD 2.1.4, 1.8.1). Every module whose option is XSI was checked against its page: the SYNOPSIS (or whole utility) is XSI-shaded for every page. The 139 assertions in Base modules were checked one by one against the shaded phrase (table in 3); Issue 8 change histories that move items to Base (pread, truncate, waitid, getrlimit but not RLIMIT_CPU, catopen but not NLSPATH, `ls` -m -n -p -s -x -f, `who`, `id_t` in `<sys/types.h>`) do not touch any gated assertion. |
| IEC 60559 floating point not supported or not claimed | 218 (123 + 93 + 2) | OK | XBD 1.8.1: the MX code "mandated by the ISO C standard only for implementations that define `__STDC_IEC_559__`". All 218 tests use one test: `__STDC_IEC_559__` or its C23 successor `__STDC_IEC_60559_BFP__` (every C test checked; no test uses only one). The system defines neither, so the option is not claimed. No Issue 8 text makes it mandatory (fenv.h, math.h say "if the implementation supports the IEC 60559 option"). NOTINUSE is right. |
| option MSG not claimed | 92 | OK | `_POSIX_MESSAGE_PASSING` -1; every `mq_*` page and `<mqueue.h>` carry MSG; `_POSIX_MQ_*` minima (XBD limits.h.002) and the MSG cancellation, fork and `_Exit` items are MSG items. Still an option in Issue 8. |
| `<threads.h>` is not provided | 73 | **TEST-BUG, fixed** (hid a SYSTEM deviation) | XBD `<threads.h>`: "Implementations shall not define the macro `__STDC_NO_THREADS__`, except for profile implementations that define `_POSIX_SUBPROFILE`". The system defines the macro and does not define `_POSIX_SUBPROFILE` (XBD.unistd.h.007 fails on it), so the header is mandatory and absent. The tests said NOTINUSE. See 4. |
| option ADV not claimed | 28 | OK | `_POSIX_ADVISORY_INFO` -1; `posix_fadvise`, `posix_fallocate`, `posix_madvise`, `posix_memalign` and their header items carry ADV in this edition. |
| option TSH not claimed | 27 | OK (XSH.fork.014 notes SI-7) | `_POSIX_THREAD_PROCESS_SHARED` -1; pshared attribute functions and the robust process-shared tests need TSH (XSI would require it, XBD 2.1.4.1, but XSI is not claimed). |
| option TPS not claimed | 23 | OK | `_POSIX_THREAD_PRIORITY_SCHEDULING` -1; pages carry TPS; the Threads option itself is Base in Issue 8 (change history) but TPS stays an option. |
| option PS, PS\|TPS, SPN+PS, TPS+PS | 42 | OK | `_POSIX_PRIORITY_SCHEDULING` -1; `sched_*` pages carry PS (`sched_get_priority_*`, `sched_rr_get_interval` carry PS or TPS); `posix_spawnattr_*sched*` carry "SPN PS". |
| option XSI\|SIO, SIO, MLR+XSI\|MLR+SIO, ML+XSI\|ML+SIO, XSI+MLR\|SIO+MLR | 26 | OK (note) | `_POSIX_SYNCHRONIZED_IO` -1: `fdatasync`, O_DSYNC and O_RSYNC are SIO (page and fcntl.h shading). `msync` is XSI-shaded; its APPLICATION USAGE says "only supported if" SIO, so the assertions accept XSI or SIO. The `mlock`/`mlockall` tests can only observe a lock through `msync` (EBUSY), so with neither claimed they cannot run although ML and MLR are claimed. The skip message names the whole expression, which is accurate. |
| option TPP\|TPI\|RPP\|RPI and relatives | 13 | OK | the four mutex priority options are all -1; MC1 is defined in XBD 1.8.1 as exactly that expression. |
| option SD+XSI not claimed | 6 | OK | SD is claimed, XSI is not: `ar -m`, `-s`, `-p`, `-q`... are XSI-shaded inside the SD utility. |
| option CD not claimed | 3 | OK | `_POSIX2_C_DEV` -1; `lex`, `yacc`, `xgettext` carry CD. |
| option PIO not claimed | 1 | OK (disputed Q-153) | `_POSIX_PRIORITIZED_IO` -1. |
| the database has no entry; holds no descriptor | 13 | OK | the standard guarantees no entry in the host/network databases; these tests are designed NOTINUSE when there is none or when no descriptor is held. |
| locale creation not claimed (`_SC_2_LOCALEDEF` -1; `POSIX2_LOCALEDEF` undefined) | 5 | OK | `_POSIX2_LOCALEDEF` is an option (XBD 2.1.3.2); it is required only of XSI systems (XBD 2.1.4.2), and XSI is not claimed. The C helper (`sysconf`) and the shell helper (`getconf`) agree here. |
| `freopen(NULL, ...)` not permitted (EFAULT) | 3 | OK | the page makes it implementation-defined "which changes of mode are permitted (if any)". |
| EPIPE instead of the pending error | 1 | OK | designed tolerance for the several errors XSH 2.3 allows. |

Reasons present only in the full run, same derivation, same verdict: CD (59
tests in all), locale creation (91), TYM (8), `SS|TSP` (6), FR (6),
`XSI+CRYPT` (8), `UP+XSI` (1), UU (1), DC (1): each option is -1 or undefined
and each assertion's expression matches the shading of its page.

### Gating that is stricter than the standard (TEST-BUG, 8 tests)

These assertions say in their own note that the requirement is Base, yet
`when XSI` skips them although the set-up they need is present on the system:

| ID | Requirement | XSI only for |
| --- | --- | --- |
| XSH.setuid.003, XSH.setgid.003, XSH.seteuid.002, XSH.setegid.002 | saved set-ID rules for an unprivileged process (not XSI) | `setreuid()` / `setregid()` to make the IDs differ |
| XSH.access.005, XSH.access.020 | `access()` uses real IDs (not XSI) | same |
| XSH.2.9-threads.016 | `tcdrain()` is a cancellation point (Base) | `posix_openpt()` for the terminal |
| XSH.fopen.019 | line buffering on a terminal (Base, disputed Q-33) | `posix_openpt()` |

The tests already degrade (UNRESOLVED or NOTINUSE) when the set-up is
absent, so the right gate is the set-up, not the option. Not changed here:
the driver builds XSI tests with `-D_XOPEN_SOURCE=800` only when the `when`
expression names XSI, and the repository has no build probe for these
functions (only `setgroups()`, `core/pcgroups.c`). A generic fix is a driver
build probe like the one for `pcgroups.o` that sets `needs setreuid` /
`needs pty`; this changes six assertion headers and the driver, so it is left
for a decision.

### Unclear in the standard (STD-AMBIG, 3 tests, new rows)

| ID | Issue | Row |
| --- | --- | --- |
| XSH.sysconf.012 | `{PAGE_SIZE}` is XSI in `<limits.h>`, the sysconf table does not shade `_SC_PAGE_SIZE` | Q-213 |
| XBD.4.5-directory-protection.001, .002 | section 4.5 is unshaded, the S_ISVTX bit it governs (and the EPERM errors that cite it) is XSI | Q-214 |

Already recorded and consistent with the gating: SI-7 (fork pshared item),
SI-9 and SI-10 (SA_NOCLDWAIT, SIG_IGN status), Q-184 (modem disconnect),
Q-165, Q-153, Q-33.

## 3. Sampled tests (30) and what the standard says

| Test | Skip | Standard check | Class |
| --- | --- | --- | --- |
| XSH.basename.001 | XSI | basename page SYNOPSIS XSI | OK |
| XSH.drand48.001 | XSI | page XSI | OK |
| XSH.hcreate.001 | XSI | page XSI | OK |
| XSH.getdate.001 | XSI | page XSI | OK |
| XSH.ftok.001 | XSI | page XSI | OK |
| XSH.getpriority.001 | XSI | page XSI | OK |
| XBD.dirent.h.009 | XSI | seekdir/telldir XSI | OK |
| XSH.getrlimit.002 | XSI | RLIMIT_CPU kept XSI by the Issue 8 change (rest moved to Base) | OK |
| XSH.getrlimit.015 | XSI | SIGXFSZ part of RLIMIT_FSIZE is XSI | OK |
| XSH.read.013 | XSI | EISDIR item XSI | OK |
| XSH.write.008 | XSI | SIGXFSZ clause XSI | OK |
| XSH.mbstowcs.004 | XSI | null `pwcs` clause XSI | OK |
| XSH.fprintf.040 | XSI | `C` and `S` conversions XSI | OK |
| XSH.futimens.018 | XSI | `utimes()` XSI | OK |
| XSH.lgamma.007 | XSI | `signgam` XSI | OK |
| XSH.mkdir.003 | XSI | S_ISVTX part of mode XSI | OK |
| XSH.unlink.018 | XSI | EPERM for S_ISVTX XSI | OK |
| XCU.ls.014 | XSI | `-g` XSI (the `-f -m -n -p -s -x` options moved to Base) | OK |
| XCU.xargs.010 | XSI | `-I` XSI | OK |
| XCU.ulimit.007 | XSI | `-t` XSI (RLIMIT_CPU) | OK |
| XCU.who.002 | XSI | `-q` XSI (utility itself moved to Base) | OK |
| XCU.nl.001 | XSI | whole utility XSI | OK |
| XCU.ar.012 | SD+XSI | `-m` XSI inside SD | OK |
| XSH.catopen.006 | XSI | NLSPATH text XSI (function itself Base) | OK |
| XSH.aio_fsync.001 | SIO | `O_DSYNC` clause SIO | OK |
| XBD.limits.h.002 | MSG | `_POSIX_MQ_*` minima under MSG | OK |
| XSH.pthread_mutex_consistent.002 | TSH | process-shared robust mutex | OK |
| XSH.acos.002 | IEC | MX item; macro absent | OK |
| XSH.call_once.001 | `<threads.h>` | see 4 | TEST-BUG (fixed) |
| XSH.setuid.003 | XSI | requirement Base, XSI only for `setreuid()` | TEST-BUG |

## 4. Changes made

Derivation: none needed. The claim table of the reference system is
byte-identical before and after (`./posixcheck.sh --no-selftest --only
XSH.call_once.001 --results /tmp/obclaim-before` and `-after`; the driver and
probe are untouched).

Skips hiding a mandatory interface: the 79 tests that guard on
`__STDC_NO_THREADS__` now report NOTINUSE only when `_POSIX_SUBPROFILE` is
defined, and FAIL otherwise ("... defined, but `<threads.h>` is required unless
`_POSIX_SUBPROFILE` is defined"). Five tests that included `<threads.h>`
unguarded (XSH.2.9-threads.005, .007, .014, .015, .024: UNINITIATED "build
failed" on such a system) got the same guard. Checked by building every
changed file with `-D__STDC_NO_THREADS__` (FAIL) and with
`-D__STDC_NO_THREADS__ -D_POSIX_SUBPROFILE` (NOTINUSE); on the reference
system the tests are unchanged (several run to PASS). Lint stays at 6373
problems before and after.

Documentation: `docs/standard-issues.md` rows Q-213 and Q-214; notes in
XSH.sysconf.012 and XBD.4.5-directory-protection.001/.002 citing them.

## 5. Issue 8 options and headers now mandatory that this system lacks (SYSTEM deviations)

Must not be read as "unsupported". Status in the full run:

| Item | Standard | Run result | Visible? |
| --- | --- | --- | --- |
| option symbols with the Issue 8 value 202405L; `_POSIX_VERSION` | XBD 2.1.3.1 | FAIL (XBD.unistd.h.004, .006, .007; XSH.sysconf.005 to .007) | yes |
| `_XOPEN_ENH_I18N`, `_XOPEN_SHM` not -1 | XBD `<unistd.h>` "always" | FAIL (XBD.unistd.h.008) | yes |
| `<threads.h>` (macro `__STDC_NO_THREADS__` defined) | XBD `<threads.h>` | was NOTINUSE x73; now FAIL x84 | fixed here |
| `<aio.h>`, asynchronous I/O (`_POSIX_ASYNCHRONOUS_IO` now Base) | XBD 2.1.3.1, change histories of aio_read, aio_fsync | FAIL XBD.aio.h.001 to .009, XSH.sysconf.006; the 59 XSH aio tests are UNINITIATED (build failed) | yes (Tier 0), function tests UNINITIATED |
| `<wordexp.h>` | Base | FAIL XBD.wordexp.h.001 to .005; 25 XSH.wordexp UNINITIATED | yes |
| `<iconv.h>`, `<monetary.h>` | Base | no XBD module for either header; only UNINITIATED in XSH.iconv (12), XSH.strfmon (13), XBD.7.3.3-lc-monetary | **gap**: add Tier 0 header modules like XBD.aio.h |
| `dup3`, `fexecve`, `O_CLOEXEC`, `POSIX_CLOSE_RESTART`, `SEEK_HOLE/SEEK_DATA`, `_SC_DEVICE_CONTROL` name | Issue 8 additions | FAIL (XBD.unistd.h.001, .002, .010, .019, .021, .024, .026) | yes |

Open follow-ups, not done: a Tier 0 module for `<threads.h>`, `<iconv.h>` and
`<monetary.h>` (the first would make the single deviation count once); the
build-probe gate for the eight tests above; the policy that a missing
mandatory header in a non-Tier-0 module is UNINITIATED rather than FAIL.
