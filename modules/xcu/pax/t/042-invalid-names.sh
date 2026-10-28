# pax -x pax: path records, and member names the destination cannot hold
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.042
# In the pax format a path record replaces the name of the member ("-o
# path:=renamed/x" lists renamed/x); a member whose name has a component
# longer than the destination allows is not created, by default or with -o
# invalid=bypass, a diagnostic is written and the next member is still
# extracted; with -o invalid=rename on a terminal the typed answer becomes
# the name, and only that member is asked about.
. "$PC_CORE/pc.sh"
. "$PC_CORE/pcpty.sh"
pc_begin XCU.pax.042
LC_ALL=C; export LC_ALL
pc_pty_need
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
rep() { awk -v c="$1" -v n="$2" 'BEGIN { s = ""; for (i = 0; i < n; i++) s = s c; printf "%s", s }'; }
nm=$(getconf NAME_MAX . 2>/dev/null)
case $nm in
''|*[!0-9]*) pc_unresolved "setup: NAME_MAX is not a number ($nm)" ;;
esac
[ "$nm" -le 3000 ] || pc_unresolved "setup: NAME_MAX is too large to exceed ($nm)"
LONG=$(rep a $((nm + 10)))
printf '1\n' >f1 && printf '2\n' >f2 || pc_unresolved "setup: cannot write files"
pax -w -x pax -o "path:=$LONG" -f A f1 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: pax -w with a long path record failed"
pax -w -x pax -a -f A f2 </dev/null >/dev/null 2>&1 || pc_unresolved "setup: appending f2 failed"
cat >inner.sh <<'IN'
cd out3 || exit 1
pax -r -o invalid=rename -f ../A >../i.out 2>../i.err
echo $? >../i.rc
exit 0
IN
printf 'renamed1\n' >typed
mkdir out1 out2 out3 || pc_unresolved "setup: mkdir failed"
pc_test
pax -w -x pax -o 'path:=renamed/x' -f P f1 </dev/null >/dev/null 2>err || pc_fail "-o path:=renamed/x: nonzero status"
[ "$(pax -f P 2>/dev/null)" = renamed/x ] || pc_fail "a path record did not replace the member name: '$(pax -f P 2>/dev/null)'"
for variant in default bypass; do
	d=out1; [ $variant = bypass ] && d=out2
	if [ $variant = default ]; then
		( cd $d && pax -r -f ../A >/dev/null 2>../err.$variant )
	else
		( cd $d && pax -r -o invalid=bypass -f ../A >/dev/null 2>../err.$variant )
	fi
	[ "$(ls $d)" = f2 ] || pc_fail "$variant: the destination holds '$(ls $d | cut -c1-40 | tr '\n' ' ')', expected only f2"
	[ "$(cat $d/f2 2>/dev/null)" = 2 ] || pc_fail "$variant: f2 was not extracted after the invalid member"
	[ -s err.$variant ] || pc_fail "$variant: no diagnostic for the member with the too long name"
done
pc_pty_run -i typed inner.sh
[ "$(cat i.rc 2>/dev/null)" = 0 ] || pc_fail "-o invalid=rename: exit status '$(cat i.rc 2>/dev/null)'"
grep aaaaaaaaaa pty.out >/dev/null || pc_fail "no prompt naming the invalid member was written to the terminal"
grep f2 pty.out >/dev/null && pc_fail "the valid member f2 was asked about on the terminal"
[ "$(cat out3/renamed1 2>/dev/null)" = 1 ] || pc_fail "the member was not extracted under the typed name renamed1"
[ "$(cat out3/f2 2>/dev/null)" = 2 ] || pc_fail "f2 was not extracted"
pc_pass
