# Shared by the grep faults and variants: the body of a standalone grep that
# runs the real one and alters exactly one thing, chosen by the name in FAULT.
# The mutant file writes REAL= and FAULT= above this body and puts the result
# first on PATH as "grep".
# Copyright (c) 2026 Nicholas Martin. All rights reserved.

NL='
'
mode=B c= l= q= i= n= s= v= x=
nsrc=0 nesrc=0
tmp=/tmp/pcg.$$
trap 'rm -f "$tmp" "$tmp.p" "$tmp.o"' EXIT

diag() { echo "grep: $*" >&2; }

addsrc() { # kind text
	nsrc=$((nsrc + 1))
	eval "sk$nsrc=\$1; st$nsrc=\$2"
	[ "$1" = e ] && nesrc=$((nesrc + 1))
	return 0
}

while getopts EFclqsvxine:f: o; do
	case $o in
	E) mode=E ;;
	F) mode=F ;;
	c) c=1 ;;
	l) l=1 ;;
	q) q=1 ;;
	s) s=1 ;;
	v) v=1 ;;
	x) x=1 ;;
	i) i=1 ;;
	n) n=1 ;;
	e)
		if [ "$FAULT" = e_dash ]; then
			case $OPTARG in -*) diag "invalid option -- $OPTARG"; exit 2 ;; esac
		fi
		addsrc e "$OPTARG"
		;;
	f)
		fc=$(cat "$OPTARG"; echo x) || exit 2
		fc=${fc%x}
		fc=${fc%"$NL"}
		addsrc f "$fc"
		;;
	*) exit 2 ;;
	esac
done
shift $((OPTIND - 1))
if [ "$nsrc" -eq 0 ]; then
	if [ $# -lt 1 ]; then
		case $FAULT in
		err_no_diag) ;;
		s_hides_all) [ -n "$s" ] || diag "no pattern" ;;
		err_stdout) echo "grep: no pattern" ;;
		*) diag "no pattern" ;;
		esac
		exit 2
	fi
	addsrc o "$1"
	shift
fi
if [ "$FAULT" = operand_always_pattern ] && [ "$nsrc" -gt 0 ] && [ "$nesrc" -gt 0 ] && [ $# -ge 1 ]; then
	addsrc e "$1"
	shift
fi
nf=$#
f1=${1:-}

# ---- faults that change the invocation -------------------------------------
oldmode=$mode
case $FAULT in
E_ignored) [ "$mode" = E ] && mode=B ;;
F_ignored) [ "$mode" = F ] && mode=B ;;
bre_literal) [ "$mode" = B ] && mode=F ;;
bre_as_ere) [ "$mode" = B ] && mode=E ;;
i_ignored) i= ;;
v_ignored) v= ;;
x_ignored) x= ;;
n_ignored) n= ;;
x_lost_with_i) [ -n "$i" ] && x= ;;
i_lost_with_v) [ -n "$v" ] && i= ;;
v_lost_with_x) [ -n "$x" ] && v= ;;
c_v_ignored) [ -n "$c" ] && v= ;;
c_ignored) c= ;;
c_n_lines) [ -n "$c" ] && [ -n "$n" ] && c= ;;
q_prints) q= ;;
l_lines) l= ;;
s_ignored) s= ;;
stdin_ignored) if [ "$nf" -eq 0 ]; then exit 1; fi ;;
stdin_extra) if [ "$nf" -ge 1 ]; then set -- "$@" -; fi ;;
esac

# a file operand that exists but cannot be used, or does not exist
badfile=
for f; do
	[ "$f" = - ] && continue
	if [ ! -e "$f" ] || { [ ! -r "$f" ] && [ ! -d "$f" ]; }; then badfile=$f; break; fi
done
case $FAULT in
s_only_missing)
	# -s hides only the message for a file that does not exist
	if [ -n "$s" ] && [ -n "$badfile" ] && [ -e "$badfile" ]; then s=; fi
	;;
err_stops | q_stops)
	if [ -n "$badfile" ] && { [ "$FAULT" = err_stops ] || [ -n "$q" ]; }; then
		stopat=$badfile
		keep=
		for f; do
			[ "$f" = "$badfile" ] && break
			keep="$keep $f"
		done
		set --
		for f in $keep; do set -- "$@" "$f"; done
	fi
	;;
esac

# ---- the pattern file ------------------------------------------------------
: >"$tmp.p"
first=1 k=1
while [ "$k" -le "$nsrc" ]; do
	eval "kind=\$sk$k text=\$st$k"
	use=1
	case $FAULT in
	e_first_only) [ "$k" -eq 1 ] || use= ;;
	e_last_only) [ "$k" -eq "$nsrc" ] || use= ;;
	f_ignored_with_e) [ "$kind" = f ] && [ "$nesrc" -gt 0 ] && use= ;;
	esac
	if [ -n "$use" ]; then
		sed_x=
		case $FAULT in
		list_first) [ "$kind" != f ] && sed_x='-n 1p' ;;
		list_last) [ "$kind" != f ] && sed_x='-n $p' ;;
		F_list_first) [ "$kind" != f ] && [ "$mode" = F ] && sed_x='-n 1p' ;;
		F_list_last) [ "$kind" != f ] && [ "$mode" = F ] && sed_x='-n $p' ;;
		f_first_line) [ "$kind" = f ] && sed_x='-n 1p' ;;
		f_last_line) [ "$kind" = f ] && sed_x='-n $p' ;;
		null_dropped_list) [ "$kind" != f ] && sed_x='/^$/d' ;;
		null_dropped_file) [ "$kind" = f ] && sed_x='/^$/d' ;;
		null_dropped) sed_x='/^$/d' ;;
		E_null_dropped) [ "$mode" = E ] && sed_x='/^$/d' ;;
		F_null_dropped) [ "$mode" = F ] && sed_x='/^$/d' ;;
		v_null_dropped) [ -n "$v" ] && sed_x='/^$/d' ;;
		B_null_dropped) [ "$mode" = B ] && [ "$kind" = o ] && sed_x='/^$/d' ;;
		anchored_start) [ "$mode" = B ] && sed_x='s/^/^/' ;;
		anchored_end) [ "$mode" = B ] && sed_x='s/$/$/' ;;
		x_alt_anchor) [ "$mode" = E ] && [ -n "$x" ] && sed_x='s/^/^/;s/$/$/' ;;
		x_start) [ "$mode" = B ] && [ -n "$x" ] && sed_x='s/^/^/' ;;
		x_end) [ "$mode" = B ] && [ -n "$x" ] && sed_x='s/$/$/' ;;
		null_error)
			case "$NL$text$NL" in *"$NL$NL"*) diag "empty pattern"; exit 2 ;; esac
			;;
		esac
		case $FAULT in
		x_alt_anchor) [ "$mode" = E ] && x= ;;
		x_start | x_end) [ "$mode" = B ] && x= ;;
		esac
		if [ -n "$sed_x" ]; then
			# shellcheck disable=SC2086
			printf '%s\n' "$text" | sed $sed_x >>"$tmp.p"
		else
			printf '%s\n' "$text" >>"$tmp.p"
		fi
	fi
	k=$((k + 1))
done

# ---- build the real invocation ---------------------------------------------
build() { # builds "$@" for the real grep into ARGS via set --
	:
}
opts=
[ "$mode" = E ] && opts="$opts -E"
[ "$mode" = F ] && opts="$opts -F"
[ -n "$c" ] && opts="$opts -c"
[ -n "$l" ] && opts="$opts -l"
[ -n "$q" ] && opts="$opts -q"
[ -n "$i" ] && opts="$opts -i"
[ -n "$n" ] && opts="$opts -n"
[ -n "$s" ] && opts="$opts -s"
[ -n "$v" ] && opts="$opts -v"
[ -n "$x" ] && opts="$opts -x"

errfd() { # run the real grep with the stderr handling of the fault
	case $FAULT in
	err_no_diag) "$REAL" "$@" 2>/dev/null ;;
	err_stdout) "$REAL" "$@" 2>&1 ;;
	s_hides_all) if [ -n "$s" ]; then "$REAL" "$@" 2>/dev/null; else "$REAL" "$@"; fi ;;
	*) "$REAL" "$@" ;;
	esac
}

# ---- faults that need their own loop over the files ------------------------
if [ "$FAULT" = n_no_reset ] && [ -n "$n" ] && [ -z "$c" ] && [ -z "$l" ] && [ "$nf" -gt 1 ]; then
	off=0 rc=1
	for f; do
		# shellcheck disable=SC2086
		"$REAL" $opts -f "$tmp.p" -- "$f" >"$tmp.o" 2>/dev/null
		r=$?
		[ "$r" -eq 0 ] && rc=0
		awk -v off="$off" -v f="$f" '{ i = index($0, ":"); printf "%s:%d:%s\n", f, substr($0, 1, i - 1) + off, substr($0, i + 1) }' "$tmp.o"
		off=$((off + $(awk 'END { print NR }' "$f")))
	done
	exit $rc
fi
if { [ "$FAULT" = l_repeat ] || [ "$FAULT" = l_all ]; } && [ -n "$l" ]; then
	rc=1
	for f; do
		# shellcheck disable=SC2086
		cnt=$("$REAL" -c $(echo $opts | sed 's/-l//') -f "$tmp.p" -- "$f" 2>/dev/null)
		[ "${cnt:-0}" -gt 0 ] && rc=0
		if [ "$FAULT" = l_all ]; then echo "$f"; continue; fi
		j=0
		while [ "$j" -lt "${cnt:-0}" ]; do echo "$f"; j=$((j + 1)); done
	done
	exit $rc
fi

# ---- the run ---------------------------------------------------------------
# shellcheck disable=SC2086
errfd $opts -f "$tmp.p" -- "$@" >"$tmp"
rc=$?
if [ -n "$stopat" ]; then
	case $FAULT in err_stops | q_stops) diag "$stopat: cannot open"; rc=2 ;; esac
fi
if [ "$FAULT" = q_error_status ] && [ -n "$q" ] && [ -n "$badfile" ]; then rc=2; fi

post() {
	case $FAULT in
	prefix_missing) if [ "$nf" -gt 1 ] && [ -z "$l" ]; then sed 's/^[^:]*://'; else cat; fi ;;
	prefix_dash) if [ "$nf" -gt 1 ] && [ -z "$l" ]; then sed 's/^\([^:]*\):/\1-/'; else cat; fi ;;
	prefix_space) if [ "$nf" -gt 1 ] && [ -z "$l" ]; then sed 's/^\([^:]*\):/\1: /'; else cat; fi ;;
	prefix_basename) if [ "$nf" -gt 1 ] && [ -z "$l" ]; then sed 's,^[^:]*/\([^/:]*\):,\1:,'; else cat; fi ;;
	prefix_single) if [ "$nf" -eq 1 ] && [ -z "$l" ] && [ -z "$q" ]; then sed "s|^|$f1:|"; else cat; fi ;;
	n_before_file) if [ "$nf" -gt 1 ] && [ -n "$n" ] && [ -z "$c" ]; then sed 's/^\([^:]*\):\([0-9]*\):/\2:\1:/'; else cat; fi ;;
	n_dash)
		if [ -n "$n" ] && [ -z "$c" ] && [ -z "$l" ]; then
			if [ "$nf" -gt 1 ]; then sed 's/^\([^:]*:\)\([0-9]*\):/\1\2-/'; else sed 's/^\([0-9]*\):/\1-/'; fi
		else cat; fi ;;
	n_zero_based | n_selected_count)
		if [ -n "$n" ] && [ -z "$c" ] && [ -z "$l" ]; then
			awk -v nf="$nf" -v M="$FAULT" '{
				s = $0; p = ""
				if (nf > 1) { i = index(s, ":"); p = substr(s, 1, i); s = substr(s, i + 1) }
				i = index(s, ":")
				if (M == "n_zero_based") num = substr(s, 1, i - 1) - 1
				else { cnt[p]++; num = cnt[p] }
				print p num ":" substr(s, i + 1)
			}'
		else cat; fi ;;
	c_off_by_one)
		if [ -n "$c" ]; then awk -F: -v nf="$nf" '{ if (nf > 1) print $1 ":" $2 + 1; else print $1 + 1 }'; else cat; fi ;;
	c_zero_silent)
		if [ -n "$c" ]; then awk -F: -v nf="$nf" '{ if ((nf > 1 ? $2 : $1) != 0) print }'; else cat; fi ;;
	c_to_stderr) if [ -n "$c" ]; then cat >&2; else cat; fi ;;
	c_nonl) if [ -n "$c" ]; then tr -d '\n'; else cat; fi ;;
	l_stdin_dash) if [ -n "$l" ]; then sed 's/^(standard input)$/-/'; else cat; fi ;;
	l_stdin_short) if [ -n "$l" ]; then sed 's/^(standard input)$/(stdin)/'; else cat; fi ;;
	l_stdin_bare) if [ -n "$l" ]; then sed 's/^(standard input)$/standard input/'; else cat; fi ;;
	l_first_only) if [ -n "$l" ]; then sed 1q; else cat; fi ;;
	l_colon) if [ -n "$l" ]; then sed 's/$/:/'; else cat; fi ;;
	l_nonl) if [ -n "$l" ]; then tr -d '\n'; else cat; fi ;;
	no_final_newline) printf '%s' "$(cat)" ;;
	trailing_space) sed 's/$/ /' ;;
	blank_after) cat; echo ;;
	*) cat ;;
	esac
}
post <"$tmp"

case $FAULT in
status_match_1) [ "$rc" -eq 0 ] && rc=1 ;;
status_nomatch_0) [ "$rc" -eq 1 ] && rc=0 ;;
status_nomatch_2) [ "$rc" -eq 1 ] && rc=2 ;;
err_status1) [ "$rc" -gt 1 ] && rc=1 ;;
err_status0) [ "$rc" -gt 1 ] && rc=0 ;;
s_status0) [ -n "$s" ] && [ "$rc" -gt 1 ] && rc=0 ;;
c_zero_status0) [ -n "$c" ] && [ "$rc" -eq 1 ] && rc=0 ;;
l_status1) [ -n "$l" ] && [ "$rc" -eq 0 ] && rc=1 ;;
q_match_status1) [ -n "$q" ] && [ "$rc" -eq 0 ] && rc=1 ;;
q_nomatch_status0) [ -n "$q" ] && [ "$rc" -eq 1 ] && rc=0 ;;
err_status3) [ "$rc" -gt 1 ] && rc=3 ;;
err_status255) [ "$rc" -gt 1 ] && rc=255 ;;
error_status_wins) [ -n "$badfile" ] && [ -z "$q" ] && [ "$rc" -le 1 ] && rc=2 ;;
esac
exit $rc
