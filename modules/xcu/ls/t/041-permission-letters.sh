# ls -l: the permission characters, one bit at a time, and set-group-ID
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.041
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.041
LC_ALL=POSIX; export LC_ALL
: >f || pc_unresolved "cannot create f"
chmod 2755 f || pc_unresolved "chmod 2755 failed"
m=$(ls -l f | cut -c1-10)
case $m in
-rwxr-?r-x) ;;
*) pc_unresolved "unexpected mode string $m" ;;
esac
[ "$m" = '-rwxr-sr-x' ] || pc_unresolved "the set-group-ID bit could not be set (mode $m)"
pc_test
for pair in 400:-r-------- 200:--w------- 100:---x------ 040:----r----- 020:-----w---- 010:------x--- 004:-------r-- 002:--------w- 001:---------x 000:---------- 640:-rw-r-----; do
	mode=${pair%%:*}; want=${pair#*:}
	chmod $mode f || pc_fail "chmod $mode failed"
	got=$(ls -l f | cut -c1-10)
	[ "$got" = "$want" ] || pc_fail "mode $mode is written as $got, expected $want"
done
chmod 2755 f || pc_fail "chmod 2755 failed"
got=$(ls -l f | cut -c1-10)
[ "$got" = '-rwxr-sr-x' ] || pc_fail "mode 2755 is written as $got, expected -rwxr-sr-x"
chmod 2745 f || pc_fail "chmod 2745 failed"
got=$(ls -l f | cut -c1-10)
[ "$got" = '-rwxr-Sr-x' ] || pc_fail "mode 2745 is written as $got, expected -rwxr-Sr-x"
pc_pass
