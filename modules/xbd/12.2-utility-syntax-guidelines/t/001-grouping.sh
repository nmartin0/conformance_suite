# Options without option-arguments grouped behind one hyphen-minus, then one option with an option-argument.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XBD.12.2-utility-syntax-guidelines.001
#
# Each grouped form is compared with the same options given one by one and
# with the result the options must give.
. "$PC_CORE/pc.sh"
pc_begin XBD.12.2-utility-syntax-guidelines.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'Apple\nbanana\napple\n' >f || pc_unresolved "cannot create f"
printf 'a:b\nnodelim\nc:d\n' >g || pc_unresolved "cannot create g"
printf '2\n10\n1\n' >n || pc_unresolved "cannot create n"
printf 'one\ntwo\nthree\n' >t || pc_unresolved "cannot create t"
mkdir d || pc_unresolved "cannot create d"
pc_test
chk() { # <what> <got> <separate form> <expected>
	[ "$2" = "$4" ] || pc_fail "$1: grouped form gave '$2', expected '$4'"
	[ "$2" = "$3" ] || pc_fail "$1: grouped form '$2' differs from the separate options '$3'"
}
a=$(cut -sf 1 -d : g 2>/dev/null </dev/null)
b=$(cut -s -f 1 -d : g 2>/dev/null </dev/null)
chk "cut -sf 1" "$a" "$b" "a${NL}c"
a=$(sort -nrk 1,1 n 2>/dev/null </dev/null)
b=$(sort -n -r -k 1,1 n 2>/dev/null </dev/null)
chk "sort -nrk 1,1" "$a" "$b" "10${NL}2${NL}1"
a=$(grep -ine apple f 2>/dev/null </dev/null)
b=$(grep -i -n -e apple f 2>/dev/null </dev/null)
chk "grep -ine apple" "$a" "$b" "1:Apple${NL}3:apple"
a=$(sed -ne 2p t 2>/dev/null </dev/null)
b=$(sed -n -e 2p t 2>/dev/null </dev/null)
chk "sed -ne 2p" "$a" "$b" "two"
a=$(ls -ld d 2>/dev/null </dev/null)
b=$(ls -l -d d 2>/dev/null </dev/null)
[ "$a" = "$b" ] || pc_fail "ls -ld d gave '$a' but ls -l -d d gave '$b'"
case $a in
d*" d") ;;
*) pc_fail "ls -ld d did not describe the directory d: '$a'" ;;
esac
a=$(wc -lw t 2>/dev/null </dev/null)
b=$(wc -l -w t 2>/dev/null </dev/null)
set -- $a
[ "$#" -eq 3 ] && [ "$1" = 3 ] && [ "$2" = 3 ] && [ "$3" = t ] || pc_fail "wc -lw t gave '$a', expected the line count 3, the word count 3 and t"
[ "$a" = "$b" ] || pc_fail "wc -lw t gave '$a' but wc -l -w t gave '$b'"
mkdir -pm 755 p/q/r 2>/dev/null </dev/null || pc_fail "mkdir -pm 755 p/q/r failed"
[ -d p/q/r ] || pc_fail "mkdir -pm 755 p/q/r did not create the directory"
a=$(ls -ld p/q/r 2>/dev/null </dev/null)
case $a in
drwxr-[xs]r-x*) ;;
*) pc_fail "mkdir -pm 755 left the new directory with '$a'" ;;
esac
mkdir -p x/y 2>/dev/null </dev/null || pc_unresolved "cannot create x/y"
: >x/y/z || pc_unresolved "cannot create x/y/z"
rm -rf x 2>/dev/null </dev/null || pc_fail "rm -rf x failed"
[ ! -e x ] || pc_fail "rm -rf x left x behind"
mkdir -p x2/y 2>/dev/null </dev/null || pc_unresolved "cannot create x2/y"
rm -fr x2 2>/dev/null </dev/null || pc_fail "rm -fr x2 failed"
[ ! -e x2 ] || pc_fail "rm -fr x2 left x2 behind"
set -- -ab foo -c
OPTIND=1
getopts ab: o
r1=$?
getopts ab: o
r2=$?
if [ "$r1" -ne 0 ] || [ "$r2" -ne 0 ] || [ "$o" != b ] || [ "$OPTARG" != foo ]; then
	pc_fail "getopts ab: on -ab foo gave statuses $r1 $r2, name '$o', OPTARG '$OPTARG'"
fi
pc_pass
