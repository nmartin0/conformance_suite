# ls -S and -t on symbolic links: the link itself unless -H or -L
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.ls.046
. "$PC_CORE/pc.sh"
pc_begin XCU.ls.046
LC_ALL=POSIX; export LC_ALL
mkdir d || pc_unresolved "cannot create d"
printf '%0100d' 0 >d/big && printf '%050d' 0 >d/mid || pc_unresolved "cannot create the files"
ln -s big d/lnk || pc_unresolved "ln -s failed"
: >d/t1 && : >d/t2 && touch -t 200101010000 d/t1 && touch -t 200501010000 d/t2 || pc_unresolved "cannot create the timed files"
ln -s t1 d/l1 || pc_unresolved "ln -s failed"
cd d || pc_unresolved "cannot enter d"
pc_test
# order <expected> <options and operands...>
order() {
	w=$1; shift
	got=$(ls "$@" 2>err </dev/null | tr '\n' ' ')
	[ "$got" = "$w " ] || pc_fail "ls $*: '$got', expected '$w'"
}
# Without -d, -F or -l, whether a symbolic link named as an operand is
# followed is not stated (those three options say they do not follow it), so
# -d is given where the link itself is wanted.
order 'big mid lnk' -dS big mid lnk
order 'big lnk mid' -HS big mid lnk
order 'big lnk mid' -LS big mid lnk
order 'l1 t2 t1' -dt t1 t2 l1
order 't2 l1 t1' -Ht t1 t2 l1
order 't2 l1 t1' -Lt t1 t2 l1
got=$(ls -S 2>err </dev/null | sed -n '1,3p' | tr '\n' ' ')
[ "$got" = 'big mid lnk ' ] || pc_fail "ls -S of the directory: '$got', expected 'big mid lnk '"
got=$(ls -LS 2>err </dev/null | sed -n '1,3p' | tr '\n' ' ')
[ "$got" = 'big lnk mid ' ] || pc_fail "ls -LS of the directory: '$got', expected 'big lnk mid '"
pc_pass
