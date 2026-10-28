# -t writes the command line to standard error.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.xargs.007
. "$PC_CORE/pc.sh"
pc_begin XCU.xargs.007
LC_ALL=C; export LC_ALL
nl='
'
pc_test
printf 'a b\n' | xargs -t echo pre >out 2>err || pc_fail "xargs -t failed"
v=$(cat out; printf x)
v=${v%x}
[ "$v" = "pre a b$nl" ] || pc_fail "with -t the command wrote '$v', expected 'pre a b'"
found=no
while IFS= read -r l; do
	set -- $l
	[ "$*" = "echo pre a b" ] && found=yes
done <err
[ "$found" = yes ] || pc_fail "standard error did not hold the line 'echo pre a b'"
printf 'a b\n' | xargs -t -n 1 echo >out 2>err || pc_fail "xargs -t -n 1 failed"
fa=no fb=no
while IFS= read -r l; do
	set -- $l
	[ "$*" = "echo a" ] && fa=yes
	[ "$*" = "echo b" ] && fb=yes
done <err
[ "$fa" = yes ] && [ "$fb" = yes ] || pc_fail "-t -n 1 did not trace both command lines"
pc_pass
