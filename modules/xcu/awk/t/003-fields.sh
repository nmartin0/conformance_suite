# awk splits a record into fields, rebuilds the record with OFS when a field is assigned, extends NF for a field past the end, and creates nothing when reading a missing field.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.003
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.003
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
r() { printf '%s\n' "$1" | awk "$2" 2>/dev/null; }
[ "$(r '  a   b  c ' '{ print NF ":" $1 ":" $3 }')" = "3:a:c" ] || pc_fail "default splitting ignored leading, trailing or repeated blanks wrongly"
[ "$(r 'a b c' '{ $2 = "X"; print; print NF }')" = "a X c${NL}3" ] || pc_fail "assigning a field did not rebuild the record"
[ "$(r 'a b c' '{ $5 = "E"; print; print NF }')" = "a b c  E${NL}5" ] || pc_fail "assigning past the last field did not extend the record and NF"
[ "$(r 'a b c' '{ print $(NF+2) "|" NF }')" = "|3" ] || pc_fail "reading a nonexistent field changed NF or was not empty"
[ "$(r 'a b c' '{ $0 = "x y"; print NF, $2 }')" = "2 y" ] || pc_fail "assigning \$0 did not re-split the fields"
[ "$(r 'a b c' 'BEGIN { OFS = "-" } { $1 = $1; print }')" = "a-b-c" ] || pc_fail "OFS was not used when rebuilding the record"
[ "$(r 'a b c' '{ print $NF; print $(1+1) }')" = "c${NL}b" ] || pc_fail "\$NF or a computed field number failed"
[ "$(r 'a b c' '{ $3 = ""; print NF "|" $0 "|" }')" = "3|a b |" ] || pc_fail "assigning an empty string to the last field changed NF"
[ "$(r 'a b c' '{ print $0; print $1 }')" = "a b c${NL}a" ] || pc_fail "\$0 and \$1 failed"
[ "$(r 'a b c' '{ $2 = ""; print NF; print }')" = "3${NL}a  c" ] || pc_fail "an emptied middle field changed NF or the record"
pc_pass
