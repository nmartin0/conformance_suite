# Checks that cannot be judged on every system

Some planted faults cannot be judged on the development machine, because the
unmodified test does not PASS there (`ci/mutate.sh` reports UNCHECKED). The
cause is the machine, not the test: a locale cannot be made, an optional
feature is not claimed, a utility does not meet a stated requirement, or no
available shell supports a feature. Those faults are not re-run by hand
each time, and the whole suite is not re-run for them either. They are
collected here and judged selectively, in one batch, on a system where the
test's own baseline passes.

## How to run the batch

On each target system, from the repository root:

    sh ci/mutate.sh $(cat ci/pending.list)

Add `--sut-sh /full/path/SH` (repeatable) where the shell matters (see the
table). A fault is judged only when the baseline PASSES; otherwise it stays
UNCHECKED and needs another system. Record each outcome in
`reports/` (outside the clean-room rules, never quoting the standard).

## What each entry needs

| Assertions | Needs a system where |
|---|---|
| awk 036-039, csplit 009-010, grep 009-010, mv 015-017, rm 016-018, printf 015, expr 010-012, nl 009-010, join 011, date 010, and the earlier locale-dependent modules (cut, sort, wc, tr, find, sed, cp, ls) | a locale made by the locale compiler can be selected |
| awk 040-042, mv 005, nl 002, xargs 016, touch 016 | the utility meets the requirement the test states (the faults were checked by hand against a conforming model) |
| od 011 | od accepts the XSI offset operand |
| ps 015, 016 | ps marks defunct processes in the default listing (015) and accepts blanks in heading text (016) |
| patch 015, 016 | patch makes no outfile.orig with `-b -o`, and applies several `-o` patches in sequence, as the tests state (faults checked by hand against a conforming model) |
| pr 001-003, 012-016 | pr writes the header date in the form the text requires (001-003, 012, 013, 016), accepts a zero gap with `-e` and `-i` (014), and `-i` replaces a single space (015); faults were checked by hand with a local wrapper |
| ar 007, 008, 015, 019, 020, 021 | ar supports the options and behaviours these tests state, such as `-C` (faults checked by hand against a conforming wrapper) |
| nm 008, 012 | nm supports the `-x`, `-o` and `-f` options the tests use |
| set 020, kill 011/013/014 | the user portability utilities option is claimed (kill also needs job control) |
| set 018 | the shell under test supports pipefail |
| printf 012 | the shell under test supports `%n$` conversion |
| cd 012, 013 | `--sut-sh` is a shell for which `cd ''` and `cd nosuch/..` fail |
| test 005, 009, 015 | `test` supports `<` and `>` (005); 009, 015 per the reports |
| trap 011, 012 | a shell whose `trap -p` output the baseline accepts |

compress (all assertions) and uudecode (all assertions): this machine has neither utility, so the faults were judged against a temporary stand-in. They need a system with the real utility, to confirm the baselines pass and the verdicts hold.

`tail -r` (the earlier tail module) also waits on a system where its baseline passes.

## Notes

- Hand checks already done are in `scratchpad` reports of the hardening
  batches and are summarised in `docs/hardening.md`; they do not replace
  this batch.
- When a batch is run, remove the entries it settled from `ci/pending.list`.
