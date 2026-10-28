# umask -S writes u=...,g=...,o=... for the clear bits.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.umask.003
. "$PC_CORE/pc.sh"
pc_begin XCU.umask.003
nl='
'
sameset() {
	[ "${#1}" -eq "${#2}" ] || return 1
	for _c in r w x; do
		case $2 in
		*$_c*) case $1 in *$_c*) ;; *) return 1 ;; esac ;;
		*) case $1 in *$_c*) return 1 ;; esac ;;
		esac
	done
	return 0
}
pc_test
umask 027
umask -S >out 2>/dev/null
rc=$?
[ "$rc" -eq 0 ] || pc_fail "umask -S exited $rc"
content=$(cat out; echo x)
content=${content%x}
s=${content%"$nl"}
[ "$content" != "$s" ] || pc_fail "output does not end with a newline"
case $s in
*"$nl"*) pc_fail "more than one line written" ;;
u=*,g=*,o=*) ;;
*) pc_fail "output '$s' is not of the form u=...,g=...,o=..." ;;
esac
u=${s#u=}
u=${u%%,*}
g=${s#*,g=}
g=${g%%,*}
o=${s#*,o=}
sameset "$u" rwx || pc_fail "owner part is '$u', wanted rwx"
sameset "$g" rx || pc_fail "group part is '$g', wanted rx"
sameset "$o" '' || pc_fail "other part is '$o', wanted nothing"
pc_pass
