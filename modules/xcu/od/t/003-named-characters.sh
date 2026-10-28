# od -t a writes each byte, using only its low seven bits, as the name of an ASCII control character, sp, del or the printable character itself.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.003
. "$PC_CORE/pc.sh"
pc_begin XCU.od.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb \001\177Z\000' >f1 || pc_fail "cannot create input"
printf 'a\n\311' >f2 || pc_fail "cannot create input"
pc_test
got=$(od -A n -v -t a f1 2>/dev/null </dev/null)
set -- $got
case "$*" in
"a nl b sp soh del Z nul" | "a lf b sp soh del Z nul") ;;
*) pc_fail "-t a names are wrong" ;;
esac
got=$(od -A n -v -t a f2 2>/dev/null </dev/null)
set -- $got
case "$*" in
"a nl I" | "a lf I") ;;
*) pc_fail "-t a did not use only the low seven bits of a byte" ;;
esac
pc_pass
