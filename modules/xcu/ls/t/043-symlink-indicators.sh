# ls -lF and -lp: the link type indicator and the target's type indicator
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.043
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.043
LC_ALL=POSIX; export LC_ALL
: >f1 && : >ex && chmod 755 ex && mkdir dd || pc_unresolved "cannot create the targets"
ln -s f1 s1 && ln -s dd s2 && ln -s ex s3 && ln -s nowhere s4 || pc_unresolved "ln -s failed"
# tail <output file> <name>: the line of the name from the name to its end
tail_of() { awk -v n="$2" '{ i = index($0, " " n); if (i > 0) { print substr($0, i + 1); exit } }' "$1"; }
pc_test
ls -lF s1 s2 s3 s4 >outF 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -lF exited with status $rc (a missing link target is not an error)"
[ -s err ] && pc_fail "ls -lF wrote a diagnostic: $(cat err)"
ls -lp s1 s2 s3 s4 >outP 2>err </dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "ls -lp exited with status $rc"
# The page gives three forms for the link's pathname field; the target's own
# indicator is optional, the link indicator @ comes with -F.
for pair in 's1:f1:' 's2:dd:/' 's3:ex:*' 's4:nowhere:'; do
	n=${pair%%:*}; rest=${pair#*:}
	c=${rest%%:*}; ind=${rest#*:}
	got=$(tail_of outF $n)
	if [ "$got" != "$n@ -> $c" ] && [ "$got" != "$n@ -> $c$ind" ]; then
		pc_fail "ls -lF writes '$got' for $n, expected '$n@ -> $c' with or without the indicator '$ind'"
	fi
	pind=
	[ "$ind" = / ] && pind=/
	got=$(tail_of outP $n)
	if [ "$got" != "$n -> $c" ] && [ "$got" != "$n -> $c$pind" ]; then
		pc_fail "ls -lp writes '$got' for $n, expected '$n -> $c' with or without the indicator '$pind'"
	fi
done
pc_pass
