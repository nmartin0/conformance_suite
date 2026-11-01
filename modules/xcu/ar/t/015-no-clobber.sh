# ar -x -C does not replace existing files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ar.015
# "ar -x -C a f1" (also written "-xC") leaves an existing f1 with
# its old contents but still extracts a file that does not exist;
# without -C an existing file is replaced.
# MUTANT    ar.C_ignored
# MUTANT    ar.C_separate_ignored
# MUTANT    ar.C_grouped_ignored
# MUTANT    ar.C_removes
# MUTANT    ar.C_extracts_nothing
# MUTANT    ar.noC_never_replaces
# MUTANT    ar.C_all_nothing_without_operand
# VARIANT   ar.c_skip_status
. "$PC_CORE/pc.sh"
pc_begin XCU.ar.015
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
# Members are added one at a time so that the order does not depend on how
# several new operands are ordered.
pc_mk() { pc_a=$1; shift; for pc_f; do ar -rc "$pc_a" "$pc_f" </dev/null >/dev/null 2>&1 || return 1; done; }
printf 'one\n' >f1; printf 'two\n' >f2
pc_mk a f1 f2 || pc_unresolved "setup: cannot build the archive"
printf 'changed\n' >f1
rm -f f2
pc_test
ar -x -C a f1 >out 2>err </dev/null
[ "$(cat f1)" = changed ] || pc_fail "-C let the extracted file replace an existing one"
[ -e f2 ] && pc_fail "ar -x -C a f1 extracted f2"
# A file that does not exist is still extracted.
ar -x -C a f2 >out 2>err </dev/null
[ "$(cat f2 2>/dev/null)" = two ] || pc_fail "-C prevented the extraction of a file that does not exist"
rm -f f2
ar -x -C a >out 2>err </dev/null
[ "$(cat f1)" = changed ] || pc_fail "-C without operands let an extracted file replace an existing one"
[ "$(cat f2 2>/dev/null)" = two ] || pc_fail "-C without operands did not extract the file that does not exist"
printf 'changed\n' >f1; rm -f f2
ar -xC a >out 2>err </dev/null
[ "$(cat f1)" = changed ] || pc_fail "-xC let the extracted file replace an existing one"
[ "$(cat f2 2>/dev/null)" = two ] || pc_fail "-xC did not extract the file that does not exist"
ar -x a f1 >out 2>err </dev/null || pc_fail "ar -x without -C: nonzero status"
[ "$(cat f1)" = one ] || pc_fail "without -C the existing file was not replaced"
printf 'changed\n' >f1; printf 'changed\n' >f2
ar -x a >out 2>err </dev/null || pc_fail "ar -x a without -C: nonzero status"
[ "$(cat f1)" = one ] && [ "$(cat f2)" = two ] || pc_fail "without -C the existing files were not replaced"
pc_pass
