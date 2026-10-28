# default tags file with locator lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ctags.001
# Without -x a file named tags is written to the current directory; it has a
# name, file name and slash-delimited search pattern per line (tab
# separated), with lines for a function definition, a type definition and a
# macro with arguments, whose pattern is found in the defining line, and
# nothing for #if or #ifdef.
. "$PC_CORE/pc.sh"
pc_begin XCU.ctags.001
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
TAB=$(printf '\t')
cat >t.c <<'EOS'
typedef int myint;
#define ADD(a, b) ((a) + (b))
#ifdef FOO
#endif
#if 0
#endif
int alpha(int x)
{
	return x;
}
static int beta(void)
{
	return 1;
}
EOS
chk() {
	n=$1; want=$2
	tl=$(grep "^$n$TAB" tags | sed -n 1p)
	[ -n "$tl" ] || pc_fail "no tag line for $n"
	nt=$(printf '%s\n' "$tl" | tr -cd "$TAB" | wc -c)
	[ "$nt" -eq 2 ] || pc_fail "tag line for $n does not have three tab separated fields: $tl"
	f2=$(printf '%s\n' "$tl" | cut -f2)
	f3=$(printf '%s\n' "$tl" | cut -f3)
	case "$f2" in *t.c) ;; *) pc_fail "file column for $n is $f2" ;; esac
	case "$f3" in /*/) ;; *) pc_fail "pattern for $n is not slash delimited: $f3" ;; esac
	p=${f3#/}; p=${p%/}
	p=${p#^}; p=${p%\$}
	p=$(printf '%s\n' "$p" | sed 's/\\\(.\)/\1/g')
	[ -n "$p" ] || pc_fail "empty pattern for $n"
	printf '%s\n' "$want" | grep -F -e "$p" >/dev/null || pc_fail "pattern for $n ($p) is not found in the defining line"
}
pc_test
ctags t.c </dev/null >out 2>err || pc_fail "ctags: nonzero status"
[ -f tags ] || pc_fail "no tags file in the current directory"
chk alpha 'int alpha(int x)'
chk beta 'static int beta(void)'
chk myint 'typedef int myint;'
chk ADD '#define ADD(a, b) ((a) + (b))'
if grep "^FOO$TAB" tags >/dev/null; then pc_fail "#ifdef produced a tag"; fi
pc_pass
