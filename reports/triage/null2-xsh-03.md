# null2 xsh-03

Pattern A (is*/isw* member tests, kind b): the stub returns -1, which is "non-zero", and the requirement is "returns non-zero" for members. A stub cannot be told from conformance on the members side; the non-member companion assertions (002) FAIL on the control and catch the opposite fault. Only the three ct.h copies that used strchr() in the test phase were improved (see below).
Pattern B (macros, kind a): isfinite/isinf/isnan/isnormal/isunordered/isgreater family/kill_dependency are macros (compiler classification), never a stub call.
Pattern C (NaN in, NaN out, kind b): the stub returns NaN, which is the required result.

| ID | kind | decision | reason | what changed | re-verified |
|---|---|---|---|---|---|
| XSH.iscntrl.001 | b | LEGIT | stub returns -1 = non-zero = required for members; 002 catches non-members. Test was also weaker on the control than on a real system: ct_has() used strchr(), itself stubbed in the test phase, so only NUL was scanned | ct_has() in t/ct.h now a plain loop (as in isdigit's copy) | yes (real PASS; control still PASS, as expected) |
| XSH.isdigit.001 | b | LEGIT | as Pattern A; ct.h already had the plain loop | none | yes |
| XSH.isgraph.001 | b | LEGIT | as Pattern A; ct.h already had the plain loop | none | no change needed |
| XSH.islower.001 | b | LEGIT | as Pattern A | none | no change needed |
| XSH.isprint.001 | b | LEGIT | as Pattern A (+ strchr in test phase) | ct_has() plain loop | yes |
| XSH.ispunct.001 | b | LEGIT | as Pattern A | none | no change needed |
| XSH.isspace.001 | b | LEGIT | as Pattern A (+ strchr in test phase) | ct_has() plain loop | yes |
| XSH.isupper.001 | b | LEGIT | as Pattern A | none | no change needed |
| XSH.isxdigit.001 | b | LEGIT | as Pattern A | none | no change needed |
| XSH.iswalnum.001, iswalpha.001, iswblank.001, iswcntrl.001, iswdigit.001, iswgraph.001, iswlower.001, iswprint.001, iswpunct.001, iswspace.001, iswupper.001, iswxdigit.001 | b | LEGIT | as Pattern A: wc_chk() calls the function for every member, stub -1 is non-zero | none | no change needed |
| XSH.iswblank.004 | a | LEGIT (strengthened) | the required property is only "iswspace() non-zero", which -1 satisfies; the subject iswblank was never called | added a check that iswblank() is non-zero at each blank position, so the test exercises its subject | yes (real PASS; control PASS: stub non-zero is conformant) |
| XSH.iswxdigit.004 | a | LEGIT (strengthened) | relations iswalnum/iswdigit/iswalpha non-zero, satisfied by -1; subject not called | added iswxdigit() non-zero check | yes (same) |
| XSH.isfinite.001, .002, .003 | a | LEGIT | macro, evaluated in the compiler; no library function exists to stub; tests compute real values and check them | none | no change needed |
| XSH.isgreater.001-006, .008, .009 | a | LEGIT | macros; results and single evaluation are really checked (a faulty macro FAILs) | none | no change needed |
| XSH.isinf.001, .002, .003 | a | LEGIT | macro | none | no change needed |
| XSH.isnan.001, .002 | a | LEGIT | macro | none | no change needed |
| XSH.isnormal.001, .002, .003 | a | LEGIT | macro; .003 ends NOTINUSE when no subnormals | none | no change needed |
| XSH.isunordered.001, .002 | a | LEGIT | macro | none | no change needed |
| XSH.kill_dependency.001 | a | LEGIT | macro of <stdatomic.h>; absence is a FAIL branch, values are compared | none | no change needed |
| XSH.ldexp.004, XSH.logb.006, XSH.nearbyint.005 | b | LEGIT | the required result is a NaN and the stub returns NaN; nothing else is stated (no errno required for a quiet NaN) | none | no change needed |
| XSH.mbsinit.001, .002 | b | LEGIT | required result is "non-zero"; stub -1 is non-zero; no negative form in these assertions (other mbsinit assertions cover non-initial states) | none | no change needed |

No new helper in core/. Edited: modules/xsh/{iscntrl,isprint,isspace}/t/ct.h, modules/xsh/iswblank/t/004*.c, modules/xsh/iswxdigit/t/004*.c.
