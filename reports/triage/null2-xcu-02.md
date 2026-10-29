# null2 triage, group xcu-02

Pattern: for read, readonly, return, set, shift and sh the subject is the shell itself.
The real-shell control keeps the real shell (the stub `sh` execs it; built-ins are never
stubbed), so these tests PASS there by construction. In the default control (null shell)
they are UNINITIATED. Each test was read and checked to FAIL on a shell that exits 0 with no
output (they compare real variable values / output / non-zero statuses), except set.012.

| ID | kind | decision | reason | change | re-verified |
|---|---|---|---|---|---|
| XCU.read.006/007/008/012/013 | a | LEGIT | built-in of the kept real shell; each checks variable values/status a null shell cannot produce | none | n/a |
| XCU.readonly.001/002/003/006/007/008 | a | LEGIT | run the real shell on a script; compare output and non-zero statuses | none | n/a |
| XCU.return.001/002/003/006 | a | LEGIT | built-in; test checks status values and output | none | n/a |
| XCU.set.001,002,004,005,007,008,009,010,011,013,014,016,017,019 | a | LEGIT | built-in of the kept shell; all require distinctive output/status/diagnostic | none | n/a |
| XCU.set.012 | a | VACUOUS | `set -n` requires no output, status 0 and no file: a shell doing nothing passes | script now starts with `echo before`; test requires exactly that output, status 0, no side effect | yes (real machine PASS; null shell now fails) |
| XCU.shift.001-005 | a | LEGIT | built-in; checks counts, params, non-zero status/warning | none | n/a |
| XCU.sh.001,003,004,007,009,010,011,012,013,016,018 | b | LEGIT | subject is the shell; the control's stub sh execs the real shell, so it conforms; a null shell fails each (output/status/diagnostic compared) | none | n/a |
| XCU.sh.006 | b | LEGIT | requirement is exit status zero for empty scripts; a null shell cannot be told apart | none | n/a |
| XCU.renice.005 | b | LEGIT | requirement is nothing on stdout (plus status 0); stub indistinguishable (the list's `says` text is garbled, the .asr line is the one-sentence form) | none | n/a |
| XCU.sleep.001 | b | LEGIT | sleep 0 must exit 0 and write nothing | none | n/a |
| XCU.split.007 | b | LEGIT | empty input: no file, status 0 | none | n/a |
| XCU.tabs.002 | b | LEGIT | success is allowed; only silent failure is forbidden, stub succeeds | none | n/a |

Counts: VACUOUS 1, LEGIT 65, WEAK 0. Lint clean for these files.
