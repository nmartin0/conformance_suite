# pax -o: precedence of -o keywords, extended headers and header fields
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.pax.037
# In list mode the value of gname shown with -o listopt follows the
# precedence: -o gname:=v beats an x record, which beats -o gname=v, which
# beats a g record, which beats the ustar header field; and -o delete=gname
# leaves the ustar header field.
. "$PC_CORE/pc.sh"
pc_begin XCU.pax.037
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
printf '12345\n' >f1 || pc_unresolved "setup: cannot write f1"
set -- $(ls -l f1)
gname=$4
pax -w -x pax -o gname=aaa f1 >A 2>/dev/null </dev/null || pc_unresolved "setup: pax -w with a global gname failed"
pax -w -x pax -o gname:=xxx f1 >X 2>/dev/null </dev/null || pc_unresolved "setup: pax -w with an x gname failed"
# show <archive> <-o options...>: the gname that listopt shows
show() { a=$1; shift; pax -v "$@" -o 'listopt=%(gname)s' <"$a" 2>err; }
# expect <value> <message> <archive> <options...>
expect() { w=$1; m=$2; shift 2; got=$(show "$@"); [ "$got" = "$w" ] || pc_fail "$m: gname shown is '$got', expected '$w'"; }
pc_test
expect aaa "a g record only" A
expect bbb "-o gname=bbb over a g record" A -o gname=bbb
expect ccc "-o gname:=ccc over a g record" A -o gname:=ccc
expect ccc "-o gname:=ccc over -o gname=bbb" A -o gname=bbb -o gname:=ccc
expect xxx "an x record only" X
expect xxx "an x record over -o gname=bbb" X -o gname=bbb
expect ccc "-o gname:=ccc over an x record" X -o gname:=ccc
case $gname in
''|*[!0-9]*)
	if [ ${#gname} -le 31 ]; then
		expect "$gname" "-o delete=gname leaves the ustar header field" A -o delete=gname
	fi ;;
esac
pc_pass
