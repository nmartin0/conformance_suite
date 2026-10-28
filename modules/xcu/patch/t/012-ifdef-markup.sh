# patch -D name marks the changes with preprocessor conditionals so that the file reads as the patched version when the name is defined and as the original when it is not.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.patch.012
. "$PC_CORE/pc.sh"
pc_begin XCU.patch.012
LC_ALL=POSIX
export LC_ALL
NL='
'
base() { printf 'a\nb\nc\nd\ne\n' >"${1:-t.txt}" || pc_unresolved "cannot create input"; }
want5() { # $1 file, rest: expected lines
	f=$1
	shift
	exp=
	for l in "$@"; do exp=$exp$l$NL; done
	got=$(cat "$f" 2>/dev/null; printf x)
	[ "$got" = "${exp}x" ]
}
cppeval() { # file, macro name, 1 if defined else 0; a tiny preprocessor for one level of #ifdef/#ifndef/#else/#endif
	file=$1
	nm=$2
	isdef=$3
	pout=
	active=1
	while IFS= read -r l; do
		case $l in
		"#ifdef $nm"*) if [ "$isdef" -eq 1 ]; then active=1; else active=0; fi ;;
		"#ifndef $nm"*) if [ "$isdef" -eq 1 ]; then active=0; else active=1; fi ;;
		"#else"*) active=$((1 - active)) ;;
		"#endif"*) active=1 ;;
		*) [ "$active" -eq 1 ] && pout=$pout$l$NL ;;
		esac
	done <"$file"
	printf '%s' "$pout"
}
printf '3c3\n< c\n---\n> X\n' >pchg || pc_unresolved "cannot create patch"
printf '2d1\n< b\n' >pdel || pc_unresolved "cannot create patch"
printf '2a3\n> Y\n' >padd || pc_unresolved "cannot create patch"
pc_test
base
patch -D MARK -i pchg t.txt >/dev/null 2>&1 </dev/null
[ $? -eq 0 ] || pc_fail "-D with a change: status not 0"
case $(cat t.txt) in
*MARK*) ;;
*) pc_fail "-D with a change: the macro name does not appear in the file" ;;
esac
[ "$(cppeval t.txt MARK 1; printf x)" = "a${NL}b${NL}X${NL}d${NL}e${NL}x" ] || pc_fail "-D change: with MARK defined the result is not a b X d e"
[ "$(cppeval t.txt MARK 0; printf x)" = "a${NL}b${NL}c${NL}d${NL}e${NL}x" ] || pc_fail "-D change: with MARK undefined the result is not the original a b c d e"
base
patch -D MARK -i pdel t.txt >/dev/null 2>&1 </dev/null
[ "$(cppeval t.txt MARK 1; printf x)" = "a${NL}c${NL}d${NL}e${NL}x" ] || pc_fail "-D delete: with MARK defined the result is not a c d e"
[ "$(cppeval t.txt MARK 0; printf x)" = "a${NL}b${NL}c${NL}d${NL}e${NL}x" ] || pc_fail "-D delete: with MARK undefined the result is not the original"
base
patch -D MARK -i padd t.txt >/dev/null 2>&1 </dev/null
[ "$(cppeval t.txt MARK 1; printf x)" = "a${NL}b${NL}Y${NL}c${NL}d${NL}e${NL}x" ] || pc_fail "-D add: with MARK defined the result is not a b Y c d e"
[ "$(cppeval t.txt MARK 0; printf x)" = "a${NL}b${NL}c${NL}d${NL}e${NL}x" ] || pc_fail "-D add: with MARK undefined the result is not the original"
pc_pass
