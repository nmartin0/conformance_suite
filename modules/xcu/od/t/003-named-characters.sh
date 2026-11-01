# od -t a writes each byte, using only its low seven bits, as the name of an ASCII control character, sp, del or the printable character itself.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.od.003
# MUTANT    od.a_bel_bell
# MUTANT    od.a_ht_tab
# MUTANT    od.a_esc_upper
# MUTANT    od.a_upper
# MUTANT    od.a_nul_zero
# MUTANT    od.a_soh_octal
# MUTANT    od.a_vt_ff
# MUTANT    od.a_us_blank
# MUTANT    od.a_sp_name
# MUTANT    od.a_sp_underscore
# MUTANT    od.a_del_upper
# MUTANT    od.a_del_octal
# MUTANT    od.a_high_raw
# MUTANT    od.a_mask6
# VARIANT   od.lf_name
# VARIANT   od.gap3
# VARIANT   od.an_empty
# VARIANT   od.trail_blank
. "$PC_CORE/pc.sh"
pc_begin XCU.od.003
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a\nb \001\177Z\000' >f1 || pc_fail "cannot create input"
printf 'a\n\311' >f2 || pc_fail "cannot create input"
# every byte value from 0 to 127, and the same values with the high bit set
awk 'BEGIN { for (i = 0; i < 256; i++) printf "%c", i }' >fall 2>/dev/null
n=$(wc -c <fall)
[ "$n" -eq 256 ] || {
	: >fall
	i=0
	while [ $i -lt 256 ]; do
		printf "\\$(printf '%03o' $i)" >>fall
		i=$((i + 1))
	done
}
pc_test
# the names of the control characters, in order, then the printable ones
names="nul soh stx etx eot enq ack bel bs ht lf vt ff cr so si dle dc1 dc2 dc3 dc4 nak syn etb can em sub esc fs gs rs us sp"
got=$(od -A n -v -t a fall 2>/dev/null </dev/null)
set -f
set -- $got
set +f
[ $# -eq 256 ] || pc_fail "256 bytes did not give 256 names"
i=0
for want in $names; do
	i=$((i + 1))
	eval "have=\${$i}"
	case "$want" in
	lf) [ "$have" = lf ] || [ "$have" = nl ] || pc_fail "byte $((i - 1)) is not named lf or nl" ;;
	*) [ "$have" = "$want" ] || pc_fail "byte $((i - 1)) is not named $want" ;;
	esac
done
# 33 to 126 are the printable characters themselves
printable='! " # $ % & '"'"' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ? @ A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _ ` a b c d e f g h i j k l m n o p q r s t u v w x y z { | } ~'
i=33
set -f
for want in $printable; do
	i=$((i + 1))
	eval "have=\${$i}"
	[ "$have" = "$want" ] || { set +f; pc_fail "byte $((i - 1)) is not written as $want"; }
done
set +f
eval "have=\${128}"
[ "$have" = del ] || pc_fail "byte 127 is not named del"
# the high half uses only the low seven bits, so it repeats the low half
set -f
j=1
while [ $j -le 128 ]; do
	eval "lo=\${$j}"
	eval "hi=\${$((j + 128))}"
	[ "$lo" = "$hi" ] || case "$lo$hi" in
	lfnl | nllf) ;;
	*) set +f; pc_fail "byte $((j + 127)) is not written like byte $((j - 1))" ;;
	esac
	j=$((j + 1))
done
set +f
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
