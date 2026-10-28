# A mandatory option-argument may be in the same argument as its option.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.1-utility-argument-syntax.001
. "$PC_CORE/pc.sh"
pc_begin XBD.12.1-utility-argument-syntax.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'Apple\nbanana\napple\n' >f || pc_unresolved "cannot create f"
printf 'a:b\nnodelim\nc:d\n' >g || pc_unresolved "cannot create g"
printf '2\n10\n1\n' >n || pc_unresolved "cannot create n"
printf 'one\ntwo\nthree\n' >t || pc_unresolved "cannot create t"
pc_test
chk() { # <what> <got> <expected>
	[ "$2" = "$3" ] || pc_fail "$1 gave '$2', expected '$3'"
}
chk "cut -f1 -d:" "$(cut -f1 -d: g 2>/dev/null </dev/null)" "a${NL}nodelim${NL}c"
chk "cut -sf1 -d:" "$(cut -sf1 -d: g 2>/dev/null </dev/null)" "a${NL}c"
chk "sort -k1,1" "$(sort -k1,1 n 2>/dev/null </dev/null)" "1${NL}10${NL}2"
chk "head -n1" "$(head -n1 t 2>/dev/null </dev/null)" "one"
chk "tail -n1" "$(tail -n1 t 2>/dev/null </dev/null)" "three"
chk "grep -eapple" "$(grep -eapple f 2>/dev/null </dev/null)" "apple"
chk "sed -e's/o/0/'" "$(sed -e's/o/0/' t 2>/dev/null </dev/null)" "0ne${NL}tw0${NL}three"
mkdir -m755 md 2>/dev/null </dev/null || pc_fail "mkdir -m755 md failed"
a=$(ls -ld md 2>/dev/null </dev/null)
case $a in
drwxr-[xs]r-x*) ;;
*) pc_fail "mkdir -m755 md left the directory with '$a'" ;;
esac
set -- -bfoo -c
OPTIND=1
getopts b:c o
if [ "$o" != b ] || [ "$OPTARG" != foo ]; then
	pc_fail "getopts b:c on -bfoo gave name '$o', OPTARG '$OPTARG', expected b and foo"
fi
pc_pass
