# A repeated option with an option-argument: the pairs are used in command-line order.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.009
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.009
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'one\ntwo\nthree\n' >t || pc_unresolved "cannot create t"
printf 'a\n' >a || pc_unresolved "cannot create a"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
chk "sed -e s/a/b/ -e s/b/c/" "$(sed -e 's/a/b/' -e 's/b/c/' a 2>/dev/null </dev/null)" "c"
chk "sed -e s/b/c/ -e s/a/b/" "$(sed -e 's/b/c/' -e 's/a/b/' a 2>/dev/null </dev/null)" "b"
chk "grep -e one -e three" "$(grep -e one -e three t 2>/dev/null </dev/null)" "one${NL}three"
chk "grep -e three -e one" "$(grep -e three -e one t 2>/dev/null </dev/null)" "one${NL}three"
set -- -b one -b two -c -b three
OPTIND=1
seq=
while getopts b:c o; do
	case $o in
	b) seq="$seq b=$OPTARG" ;;
	c) seq="$seq c" ;;
	*) seq="$seq ?" ;;
	esac
done
[ "$seq" = " b=one b=two c b=three" ] || pc_fail "getopts b:c on -b one -b two -c -b three gave '$seq', expected the options and values in the order given"
pc_pass
