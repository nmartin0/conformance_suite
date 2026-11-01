# Shared by the compress faults and variants: a wrapper around the real
# utility that plants one fault (or one permitted choice), chosen by name.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_cm NAME ARGS...: behaves as the real utility does, except for what NAME
# says. Operands of the tests contain no blanks or glob characters.
pc_cm() (
	M=$1
	shift
	fd= fc= ff= fv= alg= nb= first= viag= lastm= ren=
	nm=0 ng=0
	OPTIND=1
	while getopts fcdvb:gm: o; do
		case $o in
		f) ff=1 ;;
		c) fc=1 ;;
		d) fd=1 ;;
		v) fv=1 ;;
		b) nb=$OPTARG ;;
		g)
			alg=gzip viag=1
			ng=$((ng + 1))
			[ -z "$first" ] && first=gzip
			;;
		m)
			alg=$OPTARG viag=
			nm=$((nm + 1))
			lastm=$OPTARG
			[ -z "$first" ] && first=$OPTARG
			;;
		*)
			command compress "$@"
			exit $?
			;;
		esac
	done
	shift $((OPTIND - 1))
	ops=$*
	set -f

	fail() {
		echo "compress: $1" >&2
		exit "${2:-1}"
	}
	app() { printf x >>"$1"; }
	sfxof() {
		case ${1:-lzw} in
		lzw) echo .Z ;;
		*) echo .gz ;;
		esac
	}
	outname() {
		if [ -n "$fd" ]; then
			case $1 in
			*.Z) echo "${1%.Z}" ;;
			*.gz) echo "${1%.gz}" ;;
			*) echo "$1" ;;
			esac
		else
			echo "$1$(sfxof "$alg")"
		fi
	}
	save() {
		mkdir -p .pcb
		for o in $ops; do [ -f "$o" ] && cp "$o" ".pcb/$o"; done
	}
	restore() {
		for o in $ops; do [ -f ".pcb/$o" ] && cp ".pcb/$o" "$o"; done
	}
	magic() { od -An -tx1 -N2 "$1" 2>/dev/null | tr -d ' \n'; }
	hasdash() {
		for o in $ops; do [ "$o" = - ] && return 0; done
		return 1
	}
	tostdout() { [ -n "$fc" ] || [ -z "$ops" ] || hasdash; }
	isgz() { case ${alg:-lzw} in gzip | deflate) return 0 ;; esac; return 1; }
	islzw() { [ "${alg:-lzw}" = lzw ]; }
	unsupp=
	case ${alg:-lzw} in lzw | gzip | deflate) ;; *) unsupp=1 ;; esac
	comp=1
	[ -n "$fd" ] && comp=
	exists=
	for o in $ops; do [ -e "$(outname "$o")" ] && exists=1; done
	firstop=${ops%% *}

	run() {
		flags=
		[ -n "$fd" ] && flags="$flags -d"
		[ -n "$fc" ] && flags="$flags -c"
		[ -n "$ff" ] && flags="$flags -f"
		[ -n "$fv" ] && flags="$flags -v"
		algarg= barg=
		[ -n "$alg" ] && algarg="-m $alg"
		[ -n "$nb" ] && barg="-b $nb"
		if [ -n "$fd" ]; then
			command compress $flags $ops
		else
			command compress $flags $algarg $barg $ops
		fi
	}

	save

	# ---- before the real run ----
	case $M in
	# option handling
	first_wins) [ -n "$first" ] && alg=$first ;;
	g_always_wins) [ "$ng" -gt 0 ] && alg=gzip ;;
	m_always_wins) [ "$nm" -gt 0 ] && alg=$lastm ;;
	multiple_error) [ $((ng + nm)) -gt 1 ] && fail "more than one algorithm option given" ;;
	# gzip output
	gzc_g) [ -n "$comp" ] && [ -n "$viag" ] && alg=lzw ren=.Z:.gz ;;
	gzc_mgzip) [ -n "$comp" ] && [ -z "$viag" ] && [ "$alg" = gzip ] && alg=lzw ren=.Z:.gz ;;
	gzc_mdeflate) [ -n "$comp" ] && [ "$alg" = deflate ] && alg=lzw ren=.Z:.gz ;;
	gzs_g) [ -n "$comp" ] && [ -n "$viag" ] && ren=.gz:.Z ;;
	gzs_mgzip) [ -n "$comp" ] && [ -z "$viag" ] && [ "$alg" = gzip ] && ren=.gz:.Z ;;
	gzs_mdeflate) [ -n "$comp" ] && [ "$alg" = deflate ] && ren=.gz:.Z ;;
	# -b
	b_lzw_rejected) [ -n "$nb" ] && islzw && fail "bad value for -b" ;;
	b_lzw_9_rejected) [ "$nb" = 9 ] && islzw && fail "bad value for -b" ;;
	b_lzw_16_rejected) [ "$nb" = 16 ] && islzw && fail "bad value for -b" ;;
	b_gz_low_rejected) [ -n "$nb" ] && isgz && [ "$nb" -lt 9 ] && fail "bad value for -b" ;;
	b_gz_1_rejected) [ "$nb" = 1 ] && isgz && fail "bad value for -b" ;;
	b_gz_9_rejected) [ "$nb" = 9 ] && isgz && fail "bad value for -b" ;;
	b_as_operand) [ -n "$nb" ] && ops="$ops $nb" nb= ;;
	# operands
	only_first_c) [ -n "$comp" ] && ops=$firstop ;;
	only_first_d) [ -n "$fd" ] && ops=$firstop ;;
	d_c_reversed)
		if [ -n "$fd" ] && [ -n "$fc" ]; then
			r=
			for o in $ops; do r="$o $r"; done
			ops=$r
		fi
		;;
	dash_literal) hasdash && fail "-: No such file or directory" ;;
	# unsupported algorithm
	unsupp_fallback) [ -n "$unsupp" ] && alg= ;;
	# content decides
	d_needs_suffix)
		if [ -n "$fd" ] && [ -n "$fc" ]; then
			for o in $ops; do
				case $o in *.Z | *.gz) ;; *) fail "$o: unknown suffix" ;; esac
			done
		fi
		;;
	lzw_unrecognized) [ -n "$fd" ] && for o in $ops; do [ "$(magic "$o")" = 1f9d ] && fail "$o: not in compressed format"; done ;;
	gzip_unrecognized) [ -n "$fd" ] && for o in $ops; do [ "$(magic "$o")" = 1f8b ] && fail "$o: not in compressed format"; done ;;
	# existing output
	overwrite_silently) [ -n "$exists" ] && ff=1 ;;
	overwrite_exit1) [ -n "$exists" ] && [ -z "$ff" ] && ff=1 ow=1 ;;
	force_ignored) ff= ;;
	esac

	# ---- the real run ----
	case $M in
	c_to_stderr) if [ -n "$fc" ]; then run >&2; else run; fi ;;
	c_nothing) if [ -n "$fc" ] && [ -z "$fd" ]; then run >/dev/null; else run; fi ;;
	noarg_stderr) if [ -z "$ops" ]; then run >&2; else run; fi ;;
	noarg_empty) if [ -z "$ops" ]; then run >/dev/null; else run; fi ;;
	dash_empty) if hasdash; then run >/dev/null; else run; fi ;;
	v_silent) if [ -n "$fv" ]; then run 2>/dev/null; else run; fi ;;
	v_stdout) if [ -n "$fv" ]; then run 2>&1; else run; fi ;;
	v_both)
		if [ -n "$fv" ]; then
			run 2>.pcb/e
			cat .pcb/e
			cat .pcb/e >&2
		else
			run
		fi
		;;
	v_nodigit)
		if [ -n "$fv" ]; then
			run 2>/dev/null
			echo "compress: file compressed" >&2
		else
			run
		fi
		;;
	quiet_noisy)
		run
		[ -z "$fv" ] && echo "compress: 1 file compressed" >&2
		;;
	refuse_no_diag) if [ -n "$exists" ] && [ -z "$ff" ]; then run 2>/dev/null; else run; fi ;;
	*) run ;;
	esac
	rc=$?

	# ---- after the real run ----
	case $M in
	# basic
	keep_input) [ -n "$comp" ] && [ -z "$fc" ] && [ "$rc" -eq 0 ] && restore ;;
	d_keeps_input) [ -n "$fd" ] && [ -z "$fc" ] && [ "$rc" -eq 0 ] && restore ;;
	suffix_lz) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do mv "$o.Z" "$o.lz" 2>/dev/null; done ;;
	status1_c) [ -n "$comp" ] && [ "$rc" -eq 0 ] && rc=1 ;;
	status1_d) [ -n "$fd" ] && [ "$rc" -eq 0 ] && rc=1 ;;
	d_file_corrupt) [ -n "$fd" ] && [ -z "$fc" ] && for o in $ops; do app "$(outname "$o")"; done ;;
	d_stdout_extra) [ -n "$fd" ] && tostdout && printf x ;;
	# standard output
	c_removes_input) [ -n "$fc" ] && for o in $ops; do rm -f "$o"; done ;;
	c_creates_file) [ -n "$fc" ] && [ -n "$comp" ] && for o in $ops; do printf x >"$o.Z"; done ;;
	c_modifies_input) [ -n "$fc" ] && for o in $ops; do app "$o"; done ;;
	d_c_removes_input) [ -n "$fc" ] && [ -n "$fd" ] && for o in $ops; do rm -f "$o"; done ;;
	# gzip
	gz_method)
		if [ -n "$comp" ] && [ -z "$fc" ] && isgz; then
			for o in $ops; do printf '\007' | dd of="$o.gz" bs=1 seek=2 conv=notrunc 2>/dev/null; done
		fi
		;;
	gz_magic)
		if [ -n "$comp" ] && [ -z "$fc" ] && isgz; then
			for o in $ops; do printf '\036' | dd of="$o.gz" bs=1 seek=0 conv=notrunc 2>/dev/null; done
		fi
		;;
	# increase
	incr_status1) [ "$rc" -eq 2 ] && rc=1 ;;
	incr_status0) [ "$rc" -eq 2 ] && rc=0 ;;
	incr_status3) [ "$rc" -eq 2 ] && rc=3 ;;
	incr_removes_input) [ "$rc" -eq 2 ] && for o in $ops; do rm -f "$o"; done ;;
	incr_modifies_input) [ "$rc" -eq 2 ] && for o in $ops; do app "$o"; done ;;
	force_no_output) [ -n "$ff" ] && [ "$rc" -eq 0 ] && for o in $ops; do rm -f "$o.gz" "$o.Z"; done ;;
	force_status2) [ -n "$ff" ] && [ "$rc" -eq 0 ] && rc=2 ;;
	# -b
	b_corrupt) [ -n "$nb" ] && [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do printf x >"$(outname "$o")"; done ;;
	# several operands
	second_kept) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do last=$o; done && [ -f ".pcb/$last" ] && cp ".pcb/$last" "$last" ;;
	swap_outputs)
		if [ -n "$comp" ] && [ -z "$fc" ]; then
			set -- $ops
			if [ $# -ge 2 ]; then
				mv "$1.Z" .pcb/swap && mv "$2.Z" "$1.Z" && mv .pcb/swap "$2.Z"
			fi
		fi
		;;
	# unsupported algorithm
	unsupp_status1) [ -n "$unsupp" ] && rc=1 ;;
	unsupp_status2) [ -n "$unsupp" ] && rc=2 ;;
	unsupp_status0) [ -n "$unsupp" ] && rc=0 ;;
	unsupp_removes_input) [ -n "$unsupp" ] && for o in $ops; do rm -f "$o"; done ;;
	unsupp_modifies_input) [ -n "$unsupp" ] && for o in $ops; do app "$o"; done ;;
	# standard input and output
	noarg_d_extra) [ -n "$fd" ] && [ -z "$ops" ] && printf x ;;
	dash_d_extra) [ -n "$fd" ] && hasdash && printf x ;;
	dash_d_fail) [ -n "$fd" ] && hasdash && rc=1 ;;
	# content decides
	lzw_corrupt) [ -n "$fd" ] && for o in $ops; do [ "$(magic "$o")" = 1f9d ] && printf x; done ;;
	gzip_corrupt) [ -n "$fd" ] && for o in $ops; do [ "$(magic "$o")" = 1f8b ] && printf x; done ;;
	# existing output
	overwrite_exit0) [ -n "$exists" ] && [ -z "$ff" ] && [ "$rc" -ne 0 ] && rc=0 ;;
	overwrite_exit1) [ -n "$ow" ] && rc=1 ;;
	refuse_removes_input) [ -n "$exists" ] && [ -z "$ff" ] && for o in $ops; do rm -f "$o"; done ;;
	refuse_truncates_old) [ -n "$exists" ] && [ -z "$ff" ] && for o in $ops; do : >"$(outname "$o")"; done ;;
	refuse_old_appended) [ -n "$exists" ] && [ -z "$ff" ] && for o in $ops; do app "$(outname "$o")"; done ;;
	force_garbage) [ -n "$exists" ] && [ -n "$ff" ] && [ "$rc" -eq 0 ] && for o in $ops; do printf x >"$(outname "$o")"; done ;;
	# metadata
	c_mode_600) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do chmod 600 "$(outname "$o")"; done ;;
	c_mode_644) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do chmod 644 "$(outname "$o")"; done ;;
	c_mtime_now) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do touch "$(outname "$o")"; done ;;
	c_mtime_epoch) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do touch -t 197001020000 "$(outname "$o")"; done ;;
	c_mtime_later) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do touch -t 200001010001 "$(outname "$o")"; done ;;
	c_mtime_earlier) [ -n "$comp" ] && [ -z "$fc" ] && for o in $ops; do touch -t 199912312359 "$(outname "$o")"; done ;;
	d_mode_600) [ -n "$fd" ] && [ -z "$fc" ] && for o in $ops; do chmod 600 "$(outname "$o")"; done ;;
	d_mtime_now) [ -n "$fd" ] && [ -z "$fc" ] && for o in $ops; do touch "$(outname "$o")"; done ;;
	d_mtime_epoch) [ -n "$fd" ] && [ -z "$fc" ] && for o in $ops; do touch -t 197001020000 "$(outname "$o")"; done ;;
	# errors
	err_removes_input) [ "$rc" -ne 0 ] && for o in $ops; do rm -f "$o"; done ;;
	err_modifies_input) [ "$rc" -ne 0 ] && for o in $ops; do [ -f "$o" ] && app "$o"; done ;;
	err_truncates_input) [ "$rc" -ne 0 ] && for o in $ops; do [ -f "$o" ] && : >"$o"; done ;;
	esac

	if [ -n "$ren" ]; then
		for o in $ops; do mv "$o${ren%:*}" "$o${ren#*:}" 2>/dev/null; done
	fi
	exit "$rc"
)
