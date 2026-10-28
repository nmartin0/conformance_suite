# command lines on standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.val.006
# With the - operand each input line is a command line, and a discrepancy is
# reported as the input line, an empty line and an indented pathname
# message.
. "$PC_CORE/pc.sh"
pc_begin XCU.val.006
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf 'one\ntwo\nthree\n' >f || pc_unresolved "setup: cannot write f"
admin -if s.f </dev/null >setup.out 2>&1 || pc_unresolved "setup: admin could not create s.f"
rm -f f
pc_test
printf '%s\n' '-m wrong s.f' | val - >out 2>err
rc=$?
[ $((rc & 1)) -ne 0 ] || pc_fail "status $rc lacks 0x01"
[ "$(sed -n 1p out)" = '-m wrong s.f' ] || pc_fail "the first output line is '$(sed -n 1p out)'"
[ -z "$(sed -n 2p out)" ] || pc_fail "the second output line is not empty"
case $(sed -n 3p out) in
' s.f: '*) ;;
*) pc_fail "the third output line is '$(sed -n 3p out)'" ;;
esac
printf '%s\n' 's.f' | val - >out 2>err
rc=$?
[ "$rc" -eq 0 ] || pc_fail "an intact file given on standard input: status $rc"
pc_pass
