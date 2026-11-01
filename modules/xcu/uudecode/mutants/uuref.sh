# Shared by the uudecode faults: the placement logic of uudecode written out
# in shell, with one planted fault chosen by name. The decoding of the data
# itself is left to the real utility (asked for standard output); the fault
# alters what is done with the decoded bytes, or the way the input is
# searched, or where the result goes.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_uuref FAULT ARGS...: behaves correctly unless FAULT applies.
# pc_uu_install FAULT: writes the same thing as a standalone program, first
# on PATH, for tests that run the utility as another user.
pc_uu_dec() {
	if [ -n "${PC_UU_REAL:-}" ]; then "$PC_UU_REAL" "$@"; else command uudecode "$@"; fi
}

pc_uuref() (
	M=$1
	shift
	LC_ALL=C
	export LC_ALL
	o=
	OPTIND=1
	while getopts o: c; do
		case $c in
		o) o=$OPTARG ;;
		*) echo "uudecode: usage: uudecode [-o outfile] [file]" >&2; exit 2 ;;
		esac
	done
	shift $((OPTIND - 1))
	t=${TMPDIR:-/tmp}/pcuu.$$
	(umask 077; mkdir "$t") || { echo "uudecode: no scratch" >&2; exit 1; }
	trap 'rm -rf "$t"' EXIT
	in=${1:-}
	[ "$M" = operand_ignored ] && in=
	if [ -n "$in" ]; then
		if [ ! -f "$in" ] || [ ! -r "$in" ]; then
			case $M in
			missing_nodiag) ;;
			missing_diag_stdout) echo "uudecode: $in: cannot open" ;;
			*) echo "uudecode: $in: cannot open" >&2 ;;
			esac
			[ "$M" = missing_status0 ] && exit 0
			exit 1
		fi
		cat "$in" >"$t/in"
	else
		if [ "$M" = stdin_none ]; then echo "uudecode: no input file" >&2; exit 1; fi
		cat >"$t/in"
	fi
	# first begin line: line number, kind, mode, name
	awk '/^begin(-base64)? [0-7]+ ./ {
		k = ($1 == "begin") ? "u" : "b"; n = $0; sub(/^[^ ]+ [^ ]+ /, "", n)
		print NR; print k; print $2; print n; exit }' "$t/in" >"$t/info"
	{ IFS= read -r bl; IFS= read -r kind; IFS= read -r mode; IFS= read -r name; } <"$t/info"
	if [ -z "$kind" ]; then echo "uudecode: no begin line" >&2; exit 1; fi
	case $M in
	lead_strict) [ "$bl" -eq 1 ] || { echo "uudecode: no begin line" >&2; exit 1; } ;;
	lead_one) [ "$bl" -le 2 ] || { echo "uudecode: no begin line" >&2; exit 1; } ;;
	b64_unsupported) [ "$kind" = b ] && { echo "uudecode: unsupported format" >&2; exit 1; } ;;
	esac
	# data lines: between the begin line and the end line
	awk -v bl="$bl" -v k="$kind" 'NR > bl { if (($0 == "end" && k == "u") || ($0 == "====" && k == "b")) exit; print }' "$t/in" >"$t/data"
	case $M in
	space_rejected) awk 'length($0) > 1 && /[ ]/ { bad = 1 } END { exit bad }' "$t/data" || { echo "uudecode: bad data" >&2; exit 1; } ;;
	backtick_rejected) awk 'length($0) > 1 && /`/ { bad = 1 } END { exit bad }' "$t/data" || { echo "uudecode: bad data" >&2; exit 1; } ;;
	term_space_rejected) awk '$0 == " " { bad = 1 } END { exit bad }' "$t/data" || { echo "uudecode: bad data" >&2; exit 1; } ;;
	term_backtick_rejected) awk '$0 == "`" { bad = 1 } END { exit bad }' "$t/data" || { echo "uudecode: bad data" >&2; exit 1; } ;;
	b64_quad_per_line) awk 'length($0) % 4 != 0 { bad = 1 } END { exit bad }' "$t/data" || { echo "uudecode: bad data" >&2; exit 1; } ;;
	b64_blank_error) awk '$0 == "" { bad = 1 } END { exit bad }' "$t/data" || { echo "uudecode: bad data" >&2; exit 1; } ;;
	b64_space_error) awk '/[ ]/ { bad = 1 } END { exit bad }' "$t/data" || { echo "uudecode: bad data" >&2; exit 1; } ;;
	esac
	pc_uu_dec -o - <"$t/in" >"$t/dec" 2>"$t/derr" || { cat "$t/derr" >&2; echo "uudecode: decoding failed" >&2; exit 1; }
	sz=$(wc -c <"$t/dec")
	sz=$((sz + 0))
	cp "$t/dec" "$t/out"
	cut_to() { dd if="$t/dec" of="$t/out" bs=1 count="$1" 2>/dev/null; }
	pad_to3() {
		cp "$t/dec" "$t/out"
		pc_r=$((sz % 3))
		[ "$pc_r" -eq 0 ] || { [ "$pc_r" -eq 1 ] && printf '\000\000' >>"$t/out" || printf '\000' >>"$t/out"; }
	}
	case $M in
	drop_last) [ "$sz" -gt 0 ] && cut_to $((sz - 1)) ;;
	append_nl) printf '\n' >>"$t/out" ;;
	r_empty_nonempty) [ "$sz" -eq 0 ] && printf '\000' >"$t/out" ;;
	r_pad3) pad_to3 ;;
	r_nul_stop)
		n0=$(od -An -v -tx1 "$t/dec" | awk '{ for (i = 1; i <= NF; i++) { if ($i == "00") { print c + 0; exit } c++ } }')
		[ -n "$n0" ] && cut_to "$n0"
		;;
	r_cr_strip) tr -d '\r' <"$t/dec" >"$t/out" ;;
	r_hibit) tr '\200-\377' '\000-\177' <"$t/dec" >"$t/out" ;;
	r_ge45_drop) [ "$sz" -ge 45 ] && cut_to $((sz - 1)) ;;
	r_cap45) [ "$sz" -gt 45 ] && cut_to 45 ;;
	r_uu_tail1) [ "$kind" = u ] && [ $((sz % 3)) -eq 1 ] && cut_to $((sz - 1)) ;;
	r_uu_tail2) [ "$kind" = u ] && [ $((sz % 3)) -eq 2 ] && cut_to $((sz - 1)) ;;
	r_b64_tail1) [ "$kind" = b ] && [ $((sz % 3)) -eq 1 ] && cut_to $((sz - 1)) ;;
	r_b64_tail2) [ "$kind" = b ] && [ $((sz % 3)) -eq 2 ] && cut_to $((sz - 1)) ;;
	b64_pad_kept) [ "$kind" = b ] && pad_to3 ;;
	b64_first_line_only) [ "$kind" = b ] && [ "$sz" -gt 57 ] && cut_to 57 ;;
	lead_output)
		[ "$bl" -gt 1 ] && { head -n $((bl - 1)) "$t/in"; cat "$t/dec"; } >"$t/out"
		;;
	esac
	[ "$M" = r_empty_nofile ] && [ "$sz" -eq 0 ] && exit 0
	dest=$name
	[ -n "$o" ] && dest=$o
	[ "$M" = o_ignored ] && dest=$name
	# where does the data go?
	is_cookie() { # $1 name, $2 source (d for the data, o for -o)
		case $1 in
		/dev/stdout)
			[ "$M" = data_devstdout_file ] && [ "$2" = d ] && return 1
			[ "$M" = o_devstdout_file ] && [ "$2" = o ] && return 1
			return 0 ;;
		-)
			[ "$M" = data_dash_file ] && [ "$2" = d ] && return 1
			[ "$M" = o_dash_file ] && [ "$2" = o ] && return 1
			return 0 ;;
		esac
		return 1
	}
	src=d
	[ -n "$o" ] && [ "$M" != o_ignored ] && src=o
	to_stdout() {
		if [ "$M" = stdout_to_stderr ]; then cat "$t/out" >&2; else cat "$t/out"; fi
		[ "$M" = stdout_nl ] && printf '\n'
		return 0
	}
	if [ "$M" = o_stdout ] && [ -n "$o" ]; then to_stdout; exit 0; fi
	if is_cookie "$dest" "$src"; then
		to_stdout
		case $M in
		o_devstdout_also_data) [ "$src" = o ] && [ "$o" = /dev/stdout ] && cat "$t/out" >"$name" ;;
		o_dash_also_data) [ "$src" = o ] && [ "$o" = - ] && cat "$t/out" >"$name" ;;
		data_dash_also_file) [ "$src" = d ] && [ "$name" = - ] && cat "$t/out" >./- ;;
		esac
		[ "$M" = status1 ] && exit 1
		exit 0
	fi
	# a name that is a cookie but is not honoured by the fault becomes a plain file here
	case $dest in
	/dev/stdout) dest=./stdout-literal ;;
	-) dest=./- ;;
	esac
	[ "$M" = no_create ] && exit 0
	[ "$M" = ro_nodiag ] && exec 2>/dev/null
	if [ -e "$dest" ]; then
		case $M in
		ow_refuse) echo "uudecode: $dest: file exists" >&2; exit 1 ;;
		ow_skip) exit 0 ;;
		ow_append) cat "$t/out" >>"$dest" || { echo "uudecode: $dest: cannot write" >&2; exit 1; } ;;
		ow_no_trunc) dd if="$t/out" of="$dest" conv=notrunc 2>/dev/null || { echo "uudecode: $dest: cannot write" >&2; exit 1; } ;;
		ro_overwrite)
			if [ ! -w "$dest" ]; then chmod u+w "$dest" 2>/dev/null; fi
			cat "$t/out" >"$dest" || { echo "uudecode: $dest: cannot write" >&2; exit 1; } ;;
		ro_replace)
			if [ ! -w "$dest" ]; then rm -f "$dest" 2>/dev/null; fi
			cat "$t/out" >"$dest" || { echo "uudecode: $dest: cannot write" >&2; exit 1; } ;;
		*) cat "$t/out" >"$dest" || {
			case $M in
			ro_nodiag) ;;
			*) echo "uudecode: $dest: cannot write" >&2 ;;
			esac
			case $M in ro_status0) exit 0 ;; esac
			exit 1
		} ;;
		esac
	else
		cat "$t/out" >"$dest" || { echo "uudecode: $dest: cannot create" >&2; exit 1; }
	fi
	[ "$M" = o_both ] && [ -n "$o" ] && cat "$t/out" >"$name"
	case $M in
	file_stdout_also) cat "$t/out" ;;
	file_stdout_nl) printf '\n' ;;
	file_stdout_diag) echo "uudecode: wrote $dest" ;;
	esac
	# the permission bits
	case $M in
	mode_ignored) ;;
	mode_umask) pc_m=$((0$mode & ~0$(umask))); chmod "$(printf '%o' "$pc_m")" "$dest" 2>/dev/null ;;
	mode_fixed644) chmod 644 "$dest" 2>/dev/null ;;
	mode_owner_only) pc_m=$((0$mode & 0700)); chmod "$(printf '%o' "$pc_m")" "$dest" 2>/dev/null ;;
	mode_decimal) chmod "$(printf '%o' "$mode")" "$dest" 2>/dev/null ;;
	mode_error)
		chmod "$mode" "$dest" 2>/dev/null || { echo "uudecode: $dest: cannot set mode" >&2; exit 1; } ;;
	*) chmod "$mode" "$dest" 2>/dev/null ;;
	esac
	[ "$M" = status1 ] && exit 1
	exit 0
)

# pc_uu_install FAULT: pc-mutbin/uudecode, a standalone program that runs
# pc_uuref with the fault; PC_UU_REAL names the real utility.
pc_uu_install() {
	pc_ui_real=
	pc_ui_ifs=$IFS
	IFS=:
	for pc_ui_d in $PATH; do
		if [ -x "$pc_ui_d/uudecode" ]; then pc_ui_real=$pc_ui_d/uudecode; break; fi
	done
	IFS=$pc_ui_ifs
	[ -n "$pc_ui_real" ] || return 1
	mkdir -p pc-mutbin || return 1
	{
		printf '#!/bin/sh\nPC_UU_REAL=%s\n' "$pc_ui_real"
		cat "${PC_MUTANT_FILE%/*}/uuref.sh"
		printf 'pc_uuref %s "$@"\n' "$1"
	} >pc-mutbin/uudecode || return 1
	chmod 755 pc-mutbin/uudecode
}

# pc_uu_install_status N: a standalone program that runs the real utility
# and turns any nonzero status into N.
pc_uu_install_status() {
	pc_ui_real=
	pc_ui_ifs=$IFS
	IFS=:
	for pc_ui_d in $PATH; do
		if [ -x "$pc_ui_d/uudecode" ]; then pc_ui_real=$pc_ui_d/uudecode; break; fi
	done
	IFS=$pc_ui_ifs
	[ -n "$pc_ui_real" ] || return 1
	mkdir -p pc-mutbin || return 1
	printf '#!/bin/sh\n"%s" "$@"\nif [ $? -ne 0 ]; then exit %s; fi\nexit 0\n' "$pc_ui_real" "$1" >pc-mutbin/uudecode || return 1
	chmod 755 pc-mutbin/uudecode
}
