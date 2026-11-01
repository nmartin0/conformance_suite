# Body of the standalone expr made by exprwrap.sh; REAL and M are set above.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
set -f
nl='
'
real() { "$REAL" "$@"; }
emit() { printf '%s\n' "$1"; }
isint() { case $1 in '' | - | *[!0-9-]* | ?*-*) return 1 ;; esac; return 0; }
isfalse() { [ -z "$1" ] || [ "$1" = 0 ]; }
isop() { case $1 in '+' | '-' | '*' | / | % | '=' | '!=' | '<' | '<=' | '>' | '>=' | '&' | '|' | ':') return 0 ;; esac; return 1; }
iscmp() { case $1 in '=' | '!=' | '<' | '<=' | '>' | '>=') return 0 ;; esac; return 1; }
fin() { # print the value, exit by the rule for the value
	emit "$1"
	if [ -z "$1" ] || [ "$1" = 0 ]; then exit 1; fi
	exit 0
}
# rewrite every argument equal to $1 into $2, then run the real one
mapop() {
	pf=$1 pt=$2
	shift 2
	n=$#
	i=0
	while [ "$i" -lt "$n" ]; do
		a=$1
		shift
		[ "$a" = "$pf" ] && a=$pt
		set -- "$@" "$a"
		i=$((i + 1))
	done
	exec "$REAL" "$@"
}
arith_only() {
	for a in "$@"; do
		case $a in
		[0-9]* | -[0-9]* | '+' | '-' | '*' | / | %) ;;
		*) return 1 ;;
		esac
	done
	return 0
}
inops() { for o in $ops; do [ "$o" = "$1" ] && return 0; done; return 1; }
# redL OPS tokens...: evaluate the operators OPS left to right, other tokens kept; result in R
redL() {
	ops=$1
	shift
	R=
	cur=$1
	shift
	while [ $# -ge 2 ]; do
		op=$1 nx=$2
		shift 2
		if inops "$op"; then
			cur=$(real -- "$cur" "$op" "$nx") || exit $?
		else
			R="$R $cur $op"
			cur=$nx
		fi
	done
	R="$R $cur"
}
revtok() { RV=; for t in "$@"; do RV="$t $RV"; done; }
# redR OPS tokens...: the same, right to left; result in R
redR() {
	ops=$1
	shift
	revtok "$@"
	set -- $RV
	R=
	cur=$1
	shift
	while [ $# -ge 2 ]; do
		op=$1 nx=$2
		shift 2
		if inops "$op"; then
			cur=$(real -- "$nx" "$op" "$cur") || exit $?
		else
			R="$R $cur $op"
			cur=$nx
		fi
	done
	R="$R $cur"
	revtok $R
	R=$RV
}

# ---- faults that rewrite the arguments ----
case $M in
add_sub) mapop '+' '-' "$@" ;;
sub_add) mapop '-' '+' "$@" ;;
mul_add) mapop '*' '+' "$@" ;;
div_mod) mapop / % "$@" ;;
mod_div) mapop % / "$@" ;;
lt_as_le) mapop '<' '<=' "$@" ;;
le_as_lt) mapop '<=' '<' "$@" ;;
gt_as_ge) mapop '>' '>=' "$@" ;;
ge_as_gt) mapop '>=' '>' "$@" ;;
eq_as_ne) mapop '=' '!=' "$@" ;;
ne_as_eq) mapop '!=' '=' "$@" ;;
lt_as_gt) mapop '<' '>' "$@" ;;
le_as_ge) mapop '<=' '>=' "$@" ;;
paren_ignored)
	n=$#
	i=0
	while [ "$i" -lt "$n" ]; do
		a=$1
		shift
		case $a in
		'(' | ')') ;;
		*) set -- "$@" "$a" ;;
		esac
		i=$((i + 1))
	done
	exec "$REAL" "$@"
	;;
octal_leading)
	n=$#
	i=0
	while [ "$i" -lt "$n" ]; do
		a=$1
		shift
		case $a in
		0[0-7] | 0[0-7][0-7] | 0[0-7][0-7][0-7]) a=$((a)) ;;
		esac
		set -- "$@" "$a"
		i=$((i + 1))
	done
	exec "$REAL" "$@"
	;;
neg_dropped)
	n=$#
	i=0
	while [ "$i" -lt "$n" ]; do
		a=$1
		shift
		case $a in
		-[0-9]*) a=${a#-} ;;
		esac
		set -- "$@" "$a"
		i=$((i + 1))
	done
	exec "$REAL" "$@"
	;;
neg_invalid)
	for a in "$@"; do
		case $a in
		-[0-9]*) [ $# -gt 1 ] && { echo "expr: syntax error" >&2; exit 2; } ;;
		esac
	done
	;;
dd_invalid)
	if [ "${1:-}" = -- ]; then echo "expr: syntax error" >&2; exit 2; fi
	;;
dd_eats_next)
	if [ "${1:-}" = -- ]; then shift; [ $# -gt 0 ] && shift; exec "$REAL" "$@"; fi
	;;
esac

# ---- faults on particular shapes of expression ----
case $M in
div_round)
	if [ $# -eq 3 ] && [ "$2" = / ] && isint "$1" && isint "$3"; then
		q=$(real -- "$1" / "$3") || exit $?
		r=$(real -- "$1" % "$3") || exit $?
		if [ $((2 * r)) -ge "$3" ]; then q=$((q + 1)); fi
		fin "$q"
	fi
	;;
prec_flat | prec_inverted | assoc_right)
	if [ $# -ge 5 ] && arith_only "$@"; then
		case $M in
		prec_flat) redL "+ - * / %" "$@" ;;
		prec_inverted) redL "+ -" "$@"; redL "* / %" $R ;;
		assoc_right) redR "* / %" "$@"; redR "+ -" $R ;;
		esac
		fin "${R# }"
	fi
	;;
cmp_string_only | cmp_fold_case | cmp_first_char | cmp_mixed_zero | cmp_strings_equal | cmp_str_reversed | cmp_true_left | cmp_false_empty)
	if [ $# -eq 3 ] && iscmp "$2"; then
		a=$1 b=$3
		case $M in
		cmp_string_only) exec "$REAL" "X$a" "$2" "X$b" ;;
		cmp_fold_case)
			a=$(printf '%s' "$a" | tr 'A-Z' 'a-z')
			b=$(printf '%s' "$b" | tr 'A-Z' 'a-z')
			exec "$REAL" "$a" "$2" "$b"
			;;
		cmp_first_char)
			if ! isint "$a" || ! isint "$b"; then
				a=${a%"${a#?}"}
				b=${b%"${b#?}"}
				exec "$REAL" "X$a" "$2" "X$b"
			fi
			;;
		cmp_mixed_zero)
			if isint "$a" && ! isint "$b"; then b=0; fi
			if ! isint "$a" && isint "$b"; then a=0; fi
			exec "$REAL" "$a" "$2" "$b"
			;;
		cmp_strings_equal)
			if ! isint "$a" && ! isint "$b"; then a=0 b=0; fi
			exec "$REAL" "$a" "$2" "$b"
			;;
		cmp_str_reversed)
			if ! isint "$a" || ! isint "$b"; then exec "$REAL" "X$b" "$2" "X$a"; fi
			;;
		cmp_true_left | cmp_false_empty)
			v=$(real -- "$a" "$2" "$b") || exit $?
			if [ "$M" = cmp_true_left ] && [ "$v" = 1 ]; then emit "$a"; exit 0; fi
			if [ "$M" = cmp_false_empty ] && [ "$v" = 0 ]; then emit ""; exit 1; fi
			;;
		esac
	fi
	;;
and_expr2 | and_as_or | and_zero_true | and_one | and_ignores_second | and_ignores_first | or_one | or_expr2 | or_zero_taken | or_null_taken | or_null_result)
	if [ $# -eq 3 ] && { [ "$2" = '&' ] || [ "$2" = '|' ]; }; then
		a=$1 b=$3
		if [ "$M" = and_zero_true ] || [ "$M" = or_zero_taken ]; then
			isfalse() { [ -z "$1" ]; }
		fi
		if [ "$M" = or_null_taken ]; then
			isfalse() { [ "$1" = 0 ]; }
		fi
		case $M in
		and_expr2) if [ "$2" = '&' ]; then if isfalse "$a" || isfalse "$b"; then fin 0; fi; fin "$b"; fi ;;
		and_as_or) if [ "$2" = '&' ]; then if ! isfalse "$a"; then fin "$a"; fi; if ! isfalse "$b"; then fin "$b"; fi; fin 0; fi ;;
		and_zero_true) if [ "$2" = '&' ]; then if isfalse "$a" || isfalse "$b"; then fin 0; fi; fin "$a"; fi ;;
		and_one) if [ "$2" = '&' ]; then if isfalse "$a" || isfalse "$b"; then fin 0; fi; fin 1; fi ;;
		and_ignores_second) if [ "$2" = '&' ]; then if isfalse "$a"; then fin 0; fi; fin "$a"; fi ;;
		and_ignores_first) if [ "$2" = '&' ]; then if isfalse "$b"; then fin 0; fi; fin "$a"; fi ;;
		or_one) if [ "$2" = '|' ]; then if ! isfalse "$a"; then fin 1; fi; if ! isfalse "$b"; then fin "$b"; fi; fin 0; fi ;;
		or_expr2) if [ "$2" = '|' ]; then if ! isfalse "$b"; then fin "$b"; fi; if ! isfalse "$a"; then fin "$a"; fi; fin 0; fi ;;
		or_zero_taken | or_null_taken) if [ "$2" = '|' ]; then if ! isfalse "$a"; then fin "$a"; fi; if ! isfalse "$b"; then fin "$b"; fi; fin 0; fi ;;
		or_null_result) if [ "$2" = '|' ]; then if [ -n "$a" ] && [ "$a" != 0 ]; then fin "$a"; fi; if [ -n "$b" ] && [ "$b" != 0 ]; then fin "$b"; fi; emit ""; exit 1; fi ;;
		esac
	fi
	;;
or_and_flat)
	if [ $# -eq 5 ] && [ "$2" = '|' ] && [ "$4" = '&' ]; then
		exec "$REAL" '(' "$1" '|' "$3" ')' '&' "$5"
	fi
	;;
or_binds_tighter)
	if [ $# -eq 5 ] && [ "$2" = '&' ] && [ "$4" = '|' ]; then
		exec "$REAL" "$1" '&' '(' "$3" '|' "$5" ')'
	fi
	;;
and_over_arith)
	if [ $# -eq 5 ] && [ "$2" = '&' ]; then
		exec "$REAL" '(' "$1" '&' "$3" ')' "$4" "$5"
	fi
	;;
colon_low_prec)
	if [ $# -eq 5 ] && [ "$4" = ':' ]; then
		exec "$REAL" '(' "$1" "$2" "$3" ')' ':' "$5"
	fi
	;;
two_operands_first)
	if [ $# -eq 2 ] && ! isop "$1" && ! isop "$2" && [ "$1" != '(' ] && [ "$2" != ')' ]; then fin "$1"; fi
	;;
open_paren_ok)
	if [ "${1:-}" = '(' ]; then shift; exec "$REAL" "$@"; fi
	;;
close_paren_ok)
	for a in "$@"; do last=$a; done
	if [ "${last:-}" = ')' ]; then n=$(($# - 1)); i=0; while [ "$i" -lt "$n" ]; do a=$1; shift; set -- "$@" "$a"; i=$((i + 1)); done; shift; exec "$REAL" "$@"; fi
	;;
empty_parens_ok)
	if [ $# -eq 2 ] && [ "$1" = '(' ] && [ "$2" = ')' ]; then fin ""; fi
	;;
missing_operand_dropped)
	for a in "$@"; do last=$a; done
	if [ $# -eq 2 ] && isop "$last" && ! isop "$1"; then fin "$1"; fi
	;;
paren_depth1)
	if [ $# -ge 2 ] && [ "$1" = '(' ] && [ "$2" = '(' ]; then echo "expr: syntax error" >&2; exit 2; fi
	;;
paren_single_empty)
	if [ $# -eq 3 ] && [ "$1" = '(' ] && [ "$3" = ')' ]; then fin ""; fi
	;;
paren_string_invalid)
	if [ $# -eq 3 ] && [ "$1" = '(' ] && [ "$3" = ')' ] && ! isint "$2"; then echo "expr: syntax error" >&2; exit 2; fi
	;;
single_string_empty)
	if [ $# -eq 1 ] && ! isint "$1"; then fin ""; fi
	;;
single_string_zero)
	if [ $# -eq 1 ] && ! isint "$1"; then fin 0; fi
	;;
single_word)
	if [ $# -eq 1 ] && ! isint "$1"; then set -f; w=${1%%[ 	]*}; fin "$w"; fi
	;;
single_trim)
	if [ $# -eq 1 ] && ! isint "$1"; then set -- $1; fin "$*"; fi
	;;
single_echo)
	if [ $# -eq 1 ] && ! isint "$1"; then printf '%b\n' "$1"; exit 0; fi
	;;
div0_result0 | div0_exit1 | div0_no_diag | mod0_result0 | mod0_exit1 | mod0_no_diag | divzero_exit3 | divzero_exit100)
	op=/
	case $M in mod0_*) op=% ;; esac
	if [ $# -eq 3 ] && { [ "$2" = "$op" ] || { [ "$2" = / -o "$2" = % ] && [ "${M#divzero}" != "$M" ]; }; } && [ "$3" = 0 ]; then
		case $M in
		*_result0) fin 0 ;;
		*_exit1) echo "expr: division by zero" >&2; exit 1 ;;
		*_no_diag) exit 2 ;;
		divzero_exit3) echo "expr: division by zero" >&2; exit 3 ;;
		divzero_exit100) echo "expr: division by zero" >&2; exit 100 ;;
		esac
	fi
	;;
match_unanchored)
	if [ $# -eq 3 ] && [ "$2" = ':' ]; then
		s=$1
		while :; do
			v=$(real "$s" : "$3"); rc=$?
			if [ "$v" != 0 ] && [ -n "$v" ]; then fin "$v"; fi
			[ -z "$s" ] && break
			s=${s#?}
		done
		fin 0
	fi
	;;
match_len_plus1 | match_fail_null | match_fail_status0 | match_group_ignored | match_group_last | match_group_whole | match_group_fail_zero | match_ere | match_empty_error)
	if [ $# -eq 3 ] && [ "$2" = ':' ]; then
		s=$1 p=$3
		hasgrp=0
		case $p in *'\('*'\)'*) hasgrp=1 ;; esac
		case $M in
		match_len_plus1)
			v=$(real "$s" : "$p"); rc=$?
			if [ $hasgrp = 0 ] && [ "$v" != 0 ]; then v=$((v + 1)); fi
			fin "$v"
			;;
		match_fail_null)
			v=$(real "$s" : "$p"); rc=$?
			if [ "$v" = 0 ]; then emit ""; exit 1; fi
			emit "$v"; exit $rc
			;;
		match_fail_status0)
			v=$(real "$s" : "$p"); rc=$?
			emit "$v"; exit 0
			;;
		match_group_ignored)
			q=$(printf '%s' "$p" | sed 's/\\[()]//g')
			exec "$REAL" "$s" : "$q"
			;;
		match_group_last)
			q=$(printf '%s' "$p" | sed 's/\\(\([^\\]*\)\\)/\1/')
			exec "$REAL" "$s" : "$q"
			;;
		match_group_whole)
			if [ $hasgrp = 1 ]; then
				q=$(printf '%s' "$p" | sed 's/\\[()]//g')
				n=$(real "$s" : "$q")
				if [ "$n" -gt 0 ]; then
					t=$(printf '%s' "$s" | cut -c1-"$n"); fin "$t"
				fi
				fin ""
			fi
			;;
		match_group_fail_zero)
			v=$(real "$s" : "$p"); rc=$?
			if [ $hasgrp = 1 ] && [ -z "$v" ]; then emit 0; exit 1; fi
			emit "$v"; exit $rc
			;;
		match_ere)
			d=$(printf '\001')
			m=$(printf '%s\n' "$s" | sed -E -n "s${d}^(${p}).*\$${d}\\1${d}p")
			if [ -n "$m" ]; then fin "${#m}"; fi
			fin 0
			;;
		match_empty_error)
			if [ -z "$s" ]; then echo "expr: syntax error" >&2; exit 2; fi
			;;
		esac
	fi
	;;
esac

# ---- faults on the result: run the real one and change what it did ----
case $M in
invalid_no_diag)
	out=$("$REAL" "$@" 2>/dev/null); rc=$?
	[ $rc -ge 2 ] || emit "$out"
	exit $rc
	;;
invalid_diag_stdout)
	out=$("$REAL" "$@" 2>&1); rc=$?
	emit "$out"
	exit $rc
	;;
diag_multiline)
	out=$("$REAL" "$@" 2>&1); rc=$?
	if [ $rc -ge 2 ]; then printf 'expr: %s\nexpr: usage: expr operand...\n' "$out" >&2; exit $rc; fi
	emit "$out"
	exit $rc
	;;
esac
out=$("$REAL" "$@")
rc=$?
case $M in
no_newline) printf '%s' "$out"; exit $rc ;;
double_newline) printf '%s\n\n' "$out"; exit $rc ;;
out_stderr) printf '%s\n' "$out" >&2; exit $rc ;;
result_abs) emit "${out#-}"; exit $rc ;;
status_always0) emit "$out"; [ $rc -le 1 ] && exit 0; exit $rc ;;
status_always1) emit "$out"; [ $rc -le 1 ] && exit 1; exit $rc ;;
status_inverted) emit "$out"; [ $rc -le 1 ] && exit $((1 - rc)); exit $rc ;;
zero_exit0) emit "$out"; [ "$out" = 0 ] && exit 0; exit $rc ;;
null_exit0) emit "$out"; [ -z "$out" ] && exit 0; exit $rc ;;
string_exit1) emit "$out"; if [ $rc -eq 0 ] && ! isint "$out"; then exit 1; fi; exit $rc ;;
false_exit2) emit "$out"; [ $rc -eq 1 ] && exit 2; exit $rc ;;
false_hidden) [ $rc -eq 1 ] || emit "$out"; exit $rc ;;
null_no_output) if [ -z "$out" ] && [ $rc -eq 1 ]; then exit 1; fi; emit "$out"; exit $rc ;;
success_warning) emit "$out"; [ $rc -le 1 ] && echo "expr: warning" >&2; exit $rc ;;
invalid_exit1) [ $rc -eq 2 ] && exit 1; emit "$out"; exit $rc ;;
invalid_exit0) [ $rc -eq 2 ] && exit 0; emit "$out"; exit $rc ;;
invalid_exit3) [ $rc -eq 2 ] && exit 3; emit "$out"; exit $rc ;;
esac
[ $rc -ge 2 ] || emit "$out"
exit $rc
