# Null-system triage, group xcu-xa-01

Control: /tmp/null3 real-shell mode (stub utilities first on PATH). Utilities the shell implements as built-ins stay real in that mode, so the 'subject never called' of the built-in modules (kill, read, readonly, return, set, shift, trap, type, ulimit, unalias, unset, wait) means only that the log does not see built-ins.

| ID | kind | decision | reason | what changed | re-verified |
| --- | --- | --- | --- | --- | --- |
| XCU.kill.004 | a | WEAK | kill is a real builtin; but a kill that fails for every process (or is missing: 127) passed | positive control added: kill -s 0 on the running shell must succeed, else UNRESOLVED | yes |
| XCU.kill.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results (exact names compared) | none | n/a |
| XCU.read.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.004 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.005 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.006 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.008 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.read.012 | a | VACUOUS | ( read v ) <in; [ v = orig ] holds when read is missing or fails | the subshell must itself see the variable set to the input line (exit status checked) | yes |
| XCU.read.013 | a | WEAK | status >1 is also what a missing read gives (127) | positive control that read works on an ordinary variable (else UNRESOLVED); the read-only variable must stay unset | yes |
| XCU.readonly.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.readonly.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.readonly.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.readonly.006 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.readonly.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.readonly.008 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.return.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.return.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.return.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.return.006 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.004 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.005 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.008 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.009 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results; the pipeline consumer was cat, a stubbed helper that hid output of the left side | consumer replaced by a shell loop | yes |
| XCU.set.010 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.011 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.012 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.013 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.014 | a | VACUOUS | the check on stderr used grep, which the control stubs (always true), so a shell writing nothing passed | check by a shell loop (no grep) | yes |
| XCU.set.016 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.set.017 | a | VACUOUS | verbose/xtrace checks used grep, stubbed in the control (always true) | check by a shell loop (no grep) | yes |
| XCU.set.019 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.shift.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.shift.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.shift.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.shift.004 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.shift.005 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.stty.006 | a | VACUOUS | stty ran, but every check used grep/cmp which the control stubs (always true); only stty's own failure would have been visible | all checks by shell loops (no grep, no cmp); control now FAILs | yes |
| XCU.tabs.002 | a | VACUOUS | tabs was started through env, a stub in the control, so tabs never ran | tabs started directly with TERM as a prefix assignment; now called; still PASS on the control, which is LEGIT: the page lets tabs succeed silently (output 'may' be written) | yes |
| XCU.tabs.003 | a | VACUOUS | tabs was started through env (a stub), so never ran; outputs of two empty runs compared equal | tabs started directly; empty output is UNRESOLVED (nothing to compare); control now UNRESOLVED | yes |
| XCU.tabs.004 | a | VACUOUS | tabs was started through env (a stub), so never ran; outputs of two empty runs compared equal | tabs started directly; empty output is UNRESOLVED (nothing to compare); control now UNRESOLVED | yes |
| XCU.tabs.005 | a | VACUOUS | tabs was started through env (a stub), so never ran; outputs of two empty runs compared equal | tabs started directly; empty output is UNRESOLVED (nothing to compare); control now UNRESOLVED | yes |
| XCU.tabs.007 | a | VACUOUS | tabs was started through env (a stub), so never ran; outputs of two empty runs compared equal | tabs started directly; empty output is UNRESOLVED (nothing to compare); control now UNRESOLVED | yes |
| XCU.trap.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.004 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.005 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.006 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.008 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.013 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.015 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.016 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.017 | a | VACUOUS | listing check used grep (stubbed, always true); dash really lists nothing, a known real FAIL hidden by the stub | check by a shell loop; control and real machine both FAIL (real verdict unchanged: FAIL, already in reports/triage/linux-xcu2.md) | yes |
| XCU.trap.018 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.trap.020 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.type.002 | a | WEAK | a type that fails for every name (or is missing) passed | positive control: type must find sh, else UNRESOLVED | yes |
| XCU.type.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.ulimit.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.ulimit.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.ulimit.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.ulimit.004 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.ulimit.005 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.ulimit.006 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.ulimit.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unalias.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unalias.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unalias.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unalias.004 | a | WEAK | an absent unalias (127) or one that always fails passed | positive control: unalias of a defined alias must succeed, else UNRESOLVED | yes |
| XCU.unalias.005 | a | WEAK | an absent unalias gives no stdout and a shell diagnostic: passed | same positive control | yes |
| XCU.unalias.006 | a | VACUOUS | ( unalias x ) then alias still defined holds when unalias does nothing or is missing | the subshell must remove the alias itself (checked inside it) | yes |
| XCU.unalias.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unset.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unset.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unset.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unset.004 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unset.005 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unset.006 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.unset.007 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.wait.001 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.wait.002 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.wait.003 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.wait.004 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |
| XCU.wait.006 | a | LEGIT | builtin of the kept real shell, so there is no utility to stub and the control cannot call it; the test checks exact observable results | none | n/a |

Counts: LEGIT 74, VACUOUS 11, WEAK 5

Patterns:
- The control stubs helper utilities too (grep, cmp, cat, env, mkdir, sleep): `grep ... || fail` and `cmp -s` always succeed there, and `env VAR=x utility` never starts the utility. Checks in the shell itself (case, read loops, [ -s ]) and prefix assignments avoid it.
- Comparing two outputs proves nothing when both are empty: now UNRESOLVED.
- 'exits >0' / 'diagnostic' assertions on a built-in passed when the built-in was missing (127) or always failed: positive controls added.
- wait.001/003/004 use sleep as the delay; the control's stub sleep returns at once, so there the control cannot show an early return, but a real sleep does (not a test defect).
- Control runs after the changes: /tmp/nullchk-xcuxa01-1-* and -2-* (partial stubs: only stty and tabs stubbed, other utilities real, confirms stty.006 fails on its own subject).
