# decimal_point in the POSIX locale with -c and -k.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.locale.006
. "$PC_CORE/pc.sh"
pc_begin XCU.locale.006
nl='
'
loc() { env -i PATH="$PATH" LC_ALL=C locale "$@" 2>/dev/null </dev/null; }
rd() { v=$(cat "$1"; printf x); v=${v%x}; }
pc_test
loc decimal_point >out || pc_fail "locale decimal_point failed"
rd out
[ "$v" = ".$nl" ] || pc_fail "locale decimal_point wrote '$(printf %s "$v" | tr '\n' '|')', expected '.'"
loc -k decimal_point >out || pc_fail "locale -k decimal_point failed"
rd out
[ "$v" = "decimal_point=\".\"$nl" ] || pc_fail "locale -k decimal_point wrote '$(printf %s "$v" | tr '\n' '|')'"
loc -c decimal_point >out || pc_fail "locale -c decimal_point failed"
rd out
[ "$v" = "LC_NUMERIC$nl.$nl" ] || pc_fail "locale -c decimal_point wrote '$(printf %s "$v" | tr '\n' '|')'"
loc -ck decimal_point >out || pc_fail "locale -ck decimal_point failed"
rd out
[ "$v" = "LC_NUMERIC${nl}decimal_point=\".\"$nl" ] || pc_fail "locale -ck decimal_point wrote '$(printf %s "$v" | tr '\n' '|')'"
loc -c LC_NUMERIC >out || pc_fail "locale -c LC_NUMERIC failed"
read -r l <out
[ "$l" = LC_NUMERIC ] || pc_fail "locale -c LC_NUMERIC starts with '$l'"
loc -k LC_NUMERIC >out || pc_fail "locale -k LC_NUMERIC failed"
f=no
while IFS= read -r l; do
	[ "$l" = 'decimal_point="."' ] && f=yes
	case $l in LC_NUMERIC*) pc_fail "locale -k LC_NUMERIC wrote the category name without -c" ;; esac
done <out
[ "$f" = yes ] || pc_fail "locale -k LC_NUMERIC did not write decimal_point"
pc_pass
