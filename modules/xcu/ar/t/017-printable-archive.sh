# An archive of printable files is printable
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.017
# An archive made only of printable text files (also after -d, -m
# and -q, and with a long member name) consists only of
# characters of the print and space classes and <backspace>
# (the definition of a printable file).
# MUTANT    ar.bin_nul
# MUTANT    ar.bin_ctrl
# MUTANT    ar.bin_del
# MUTANT    ar.bin_high
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.017
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
printf 'one\ntwo\n' >f1; printf 'three\n' >f2; printf 'odd length\n' >f3
printf 'tab\there\n' >f4; printf 'x\n' >f5
long=0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-long-member-name.txt
printf 'long\n' >"$long" 2>/dev/null || pc_notinuse "the file system does not allow a name of ${#long} bytes"
pc_mk a f1 f2 f3 f4 f5 "$long" || pc_unresolved "setup: cannot build the archive"
[ -s a ] || pc_unresolved "setup: the archive is empty"
pc_test
n=$(tr -d '[:print:][:space:]\b' <a | wc -c)
[ "$n" -eq 0 ] || pc_fail "the archive contains $n characters that are not printable"
ar -d a f2 </dev/null >/dev/null 2>&1 || pc_fail "ar -d: nonzero status"
n=$(tr -d '[:print:][:space:]\b' <a | wc -c)
[ "$n" -eq 0 ] || pc_fail "the archive contains $n characters that are not printable after a deletion"
ar -m a f1 </dev/null >/dev/null 2>&1 || pc_fail "ar -m: nonzero status"
n=$(tr -d '[:print:][:space:]\b' <a | wc -c)
[ "$n" -eq 0 ] || pc_fail "the archive contains $n characters that are not printable after a move"
ar -q a f2 </dev/null >/dev/null 2>&1 || pc_fail "ar -q: nonzero status"
n=$(tr -d '[:print:][:space:]\b' <a | wc -c)
[ "$n" -eq 0 ] || pc_fail "the archive contains $n characters that are not printable after an append"
pc_pass
