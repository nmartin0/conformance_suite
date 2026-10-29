# Null-system triage, round 2, group xsh-04

ID | kind | decision | reason | what changed | re-verified
---|---|---|---|---|---
XSH.nextafter.004 | b | LEGIT | required value is a NaN, which is exactly the stub's result; errors are other assertions | none | n/a
XSH.openlog.001 | a | WEAK | setlogmask() was called; the stub's -1 has every severity bit, so "all eight present" held for a function that answers everything | after the query, sets LOG_MASK(LOG_EMERG), requires the previous mask to equal the query and a later query to hold only that severity | yes (null: FAIL; real: PASS)
XSH.openlog.004 | a | LEGIT | only the LOG_MASK/LOG_UPTO macros (and their type); no library call exists to stub | none | n/a
XSH.openlog.005 | a | LEGIT | same: macro values only | none | n/a
XSH.openlog.009 | b | LEGIT | openlog/syslog/closelog return void and define no errors; "without failing or blocking" is satisfied by a system that does nothing | none | n/a
XSH.pthread_equal.001 | b | LEGIT | stub returns -1, which is non-zero, as required for equal IDs; the unequal case is a separate assertion | none | n/a
XSH.random.006 | a | WEAK | initstate() was called (listed as (a) only because the module subject is random()); a function that always returns NULL passed | first requires a non-null return for size 128, then the small sizes | yes (null: FAIL; real: PASS)
XSH.recvmsg.026 | a | LEGIT | CMSG_* are macros; relations between their values, no call | none | n/a
XSH.recvmsg.027 | a | LEGIT | CMSG_FIRSTHDR macro on a local msghdr | none | n/a
XSH.recvmsg.029 | a | LEGIT | compile-time properties of CMSG_LEN/CMSG_SPACE | none | n/a
XSH.remainder.003 | b | LEGIT | required value is a NaN = stub result; ordinary values are other assertions | none | n/a
XSH.remquo.003 | b | LEGIT | same | none | n/a
XSH.rint.004 | b | LEGIT | same | none | n/a
XSH.round.004 | b | LEGIT | same | none | n/a
XSH.scalbn.004 | b | LEGIT | same | none | n/a
XSH.scalbn.013 | a | LEGIT | same for scalbln (listed as (a) because the module subject is scalbn) | none | n/a
XSH.sched_get_priority_max.004 | a | LEGIT | pairwise difference of constants; no call | none | n/a
XSH.select.024 | a | LEGIT | FD_ZERO/FD_ISSET are macros working on local memory | none | n/a
XSH.select.025 | a | LEGIT | same (FD_SET) | none | n/a
XSH.select.026 | a | LEGIT | same (FD_CLR) | none | n/a
XSH.select.027 | a | LEGIT | same (FD_ISSET) | none | n/a
XSH.signal.001 | a | LEGIT | test is sound; the control missed the call: the strict-feature-macro build binds signal() to an alternate library symbol that the stub library does not define, so the real function ran. A real stub returns NULL, which fails every check here | none; tooling gap (see below) | yes, reasoned (not reproducible until the alias is stubbed)
XSH.signal.005 | a | LEGIT | same alias gap; stub returns NULL, not SIG_ERR, so it would FAIL | none | as above
XSH.signal.006 | a | LEGIT | same alias gap; stub sets errno ENOSYS, so it would FAIL | none | as above
XSH.signbit.001 | a | LEGIT | signbit is a macro; no stubbable call | none | n/a
XSH.signbit.002 | a | LEGIT | same | none | n/a
XSH.signbit.003 | a | LEGIT | same | none | n/a
XSH.signbit.004 | a | LEGIT | same | none | n/a
XSH.sin.004 | b | LEGIT | value assertion only: NaN is the stub's result; the domain error is another assertion | none | n/a
XSH.sqrt.004 | b | LEGIT | same | none | n/a
XSH.strerror.009 | a | LEGIT | alias gap: strerror_r with the strict macros binds to an alternate symbol the stub library lacks; a real stub returns -1, which is neither 0 nor EINVAL, so it would FAIL | none; tooling gap | yes, reasoned
XSH.system.001 | b | LEGIT | the requirement is only "non-zero" and the stub returns -1; nothing in the text excludes -1 | none | n/a
XSH.tgamma.004 | b | LEGIT | value assertion, NaN = stub result | none | n/a
XSH.tgamma.006 | b | LEGIT | same | none | n/a
XSH.trunc.004 | b | LEGIT | same | none | n/a
XSH.va_start.002 | a | LEGIT | va_* are macros/compiler facilities; no library function | none | n/a
XSH.va_start.003 | a | LEGIT | same | none | n/a
XSH.va_start.004 | a | LEGIT | same | none | n/a
XSH.va_start.006 | a | LEGIT | same | none | n/a
XSH.va_start.007 | a | LEGIT | same | none | n/a
XSH.vfscanf.003 | a | LEGIT | alias gap: vsscanf is bound to an alternate symbol; real stub returns -1 and fails the checks | none; tooling gap | yes, reasoned
XSH.vfscanf.004 | a | LEGIT | same | none | as above
XSH.vfscanf.005 | a | LEGIT | same alias gap for vfscanf; stub's errno is ENOSYS, not EBADF | none | as above
XSH.wordexp.025 | a | LEGIT | constants and member types of the header only | none | n/a
XSH.y0.002 | b | LEGIT | NaN value = stub result | none | n/a
XSH.y0.003 | b | LEGIT | "NaN or -HUGE_VAL" is allowed, NaN is the stub's result | none | n/a

No new helper in core/ was needed.

## Counts and patterns
VACUOUS 0, WEAK 2, LEGIT 44.

- Tooling gap (not a test defect): with the strict feature macros the C library's header redirects signal, strerror_r, vsscanf and vfscanf to alternate symbols, so the stub library never sees the call and the test runs on the real function. Fix belongs in gen/nullsys.py (define the alternates, or compile the tests so the standard names bind). Confirmed with nm on the built tests. Not edited here (outside the assertion files).
- NaN value assertions (12): the stub's NaN is the required result; the sibling assertions separate the stub (same decision as round 1).
- Macro-only assertions (CMSG_*, FD_*, signbit, va_*, LOG_MASK): the library stubs cannot reach them.
