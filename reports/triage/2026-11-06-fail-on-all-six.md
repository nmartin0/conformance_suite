# The 39 assertions that FAIL on all six systems

Selected by joining PureDarwin run3 with the other five systems' root runs
(older tree; only IDs present in both). Each was read against the standard's
text by a Sonnet reader (three readers, 13 each); nothing was run. Verdicts:
REAL = the systems deviate, TESTBUG = the test asks for more than the standard,
UNSURE = the cause is not visible from the failure note.

Totals: REAL 34, TESTBUG 1, UNSURE 4, ENV 0.

## Main finding

Most of these are not defects of six unrelated systems but additions in the 2024
edition that none of them has yet. Grouped:

- Types and structures: reclen_t (dirent.h.004, .006, sys_types.h.007, .011,
  2.11-data-types.002), struct posix_dent and posix_getdents (dirent.h.006, .008),
  struct f_owner_ex with F_GETOWN_EX/F_SETOWN_EX/F_OWNER_* (fcntl.h.008),
  F_OFD_* locks (fcntl.h.006).
- Constants and names: ALTMON_/ABALTMON_ (langinfo.h.003), REG_MINIMAL (regex.h.002),
  POSIX_CLOSE_RESTART (unistd.h.001), SEEK_HOLE/SEEK_DATA and O_CLOEXEC/O_CLOFORK
  through <unistd.h> (unistd.h.002, .021), _SC_DEVICE_CONTROL (.010), the other new
  _SC_/_PC_/_CS_ names (.019), _Fork/dup3/pipe2/getentropy/posix_close declared by
  <unistd.h> (.024).
- Option macro values: _POSIX_VERSION and _POSIX2_VERSION (unistd.h.004), the
  option symbols that must be 202405L or in {-1, 0, 202405L} (unistd.h.006, .007).
- Library behaviour: regcomp repetition modifier (regcomp.033), strftime/wcsftime
  flags 0 and + with width (strftime.007, wcsftime.006), rename with a trailing
  slash on a new name (rename.014), swprintf EOVERFLOW (fwprintf.014, an older
  requirement), fscanf %x on "0xg" (fscanf.015, medium confidence), find -iname
  with a bracket class (4.1-case-insensitive-comparisons.002).
- Utilities: dd iflags=fullblock (dd.020), m4 mkstemp failure handling (m4.016),
  trap listing of signals ignored on entry (trap.017), readlink diagnostic for a
  non-link (readlink.004), the new gettext and msgfmt utilities (gettext.002,
  msgfmt.002, msgfmt.003), pax -o keyword:=value (pax.035).

These are the suite doing its job: they measure conformance to POSIX.1-2024, and
the six systems predate it. They are not "defects to fix" in a test sense.

## Test side

- XCU.msgfmt.005 TESTBUG (medium): the .po fixtures have no header entry; the
  standard only fixes two abnormalities for -c -v and allows extra checks, so a
  conforming msgfmt may reject a header-less file. Fix: add a minimal header entry
  to the four fixtures; if it still fails it is REAL.
- Failure messages hide the cause in several tests: stderr goes to a file that is
  never shown (gettext.002, msgfmt.003, pax.025, .035, .043, strings.003,
  diff.003). Include the status and the stderr text in the failure message and
  check `command -v` for new utilities, so "utility missing" is told from "option
  rejected".
- XCU.strings.003: the `-t o` and `-t x` normalisation strips fewer blanks than the
  `-t d` one; make the three consistent.
- XBD.unistd.h.001/.006/.007/.019/.024 and fcntl.h.006 stop at the first missing
  name; one check per name would show which each system lacks (also separates
  "missing" from "hidden by a feature macro").
- XSH.rename.014: the header comment says "not a directory" where the new name
  simply does not exist; reword. The check itself is right.

## UNSURE (need the err file or a guest probe)

- XCU.diff.003 (header timestamps of -c; test sound, cause not visible)
- XCU.pax.025 (`-s ',f,&&,'`; test sound, cause not visible)
- XCU.pax.043 (first step `pax -w -x pax f1 h` fails before linkdata is examined)
- XCU.strings.003 (offsets right; all six fail the `-t d` check for an unseen reason)

## All 39

REAL: XBD.4.1-case-insensitive-comparisons.002, XBD.dirent.h.004, .006, .008,
XBD.fcntl.h.006, .008, XBD.langinfo.h.003, XBD.regex.h.002, XBD.sys_types.h.007,
.011, XBD.unistd.h.001, .002, .004, .006, .007, .010, .019, .021, .024,
XCU.dd.020, XCU.gettext.002, XCU.m4.016, XCU.msgfmt.002, .003, XCU.pax.035,
XCU.readlink.004, XCU.trap.017, XSH.2.11-data-types.002, XSH.fscanf.015,
XSH.fwprintf.014, XSH.regcomp.033, XSH.rename.014, XSH.strftime.007,
XSH.wcsftime.006.
TESTBUG: XCU.msgfmt.005.
UNSURE: XCU.diff.003, XCU.pax.025, XCU.pax.043, XCU.strings.003.

Confidence is high for the "standard newer" group and lower for the utilities
whose stderr is not visible (gettext.002, msgfmt.002/.003, m4.016, trap.017, pax.035).
