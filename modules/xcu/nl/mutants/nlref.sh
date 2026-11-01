# Shared by the nl faults and variants: a small reimplementation of nl for
# the POSIX locale (a character is a byte) with one planted fault or choice,
# selected by name. The caller sets PC_NLREF_DIR to the directory of this file.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_nlref NAME ARGS...: behaves correctly unless NAME applies.
pc_nlref() (
	M=$1
	shift
	bt=t ht=n ft=n incr=1 lnum=1 fmt=rn sep=$(printf '\t') start=1 width=6
	d1='\' d2=':' pflag= dgiven=
	case $M in
	default_width5) width=5 ;;
	default_width7) width=7 ;;
	default_left) fmt=ln ;;
	default_zero) fmt=rz ;;
	default_sep_space) sep=' ' ;;
	default_a) bt=a ;;
	default_start0) start=0 ;;
	default_incr2) incr=2 ;;
	hdr_numbered) ht=a ;;
	ftr_numbered) ft=a ;;
	esac
	OPTIND=1
	bad=
	while getopts ':pb:d:f:h:i:l:n:s:v:w:' o; do
		case $o in
		p) pflag=1 ;;
		b)
			bt=$OPTARG
			[ "$M" = b_sets_all ] && { ht=$OPTARG; ft=$OPTARG; }
			;;
		h)
			ht=$OPTARG
			[ "$M" = h_sets_body ] && bt=$OPTARG
			;;
		f)
			ft=$OPTARG
			[ "$M" = f_sets_header ] && ht=$OPTARG
			;;
		d) d=$OPTARG dgiven=1 ;;
		i) incr=$OPTARG ;;
		l) lnum=$OPTARG ;;
		n) fmt=$OPTARG ;;
		s) sep=$OPTARG ;;
		v) start=$OPTARG ;;
		w) width=$OPTARG ;;
		*) bad=1 ;;
		esac
	done
	if [ -n "$bad" ]; then
		case $M in
		optarg_accepted) ;;
		optarg_nodiag) exit 2 ;;
		optarg_status0) echo "nl: bad option or missing option argument" >&2; exit 0 ;;
		optarg_stdout) echo "nl: bad option or missing option argument"; exit 2 ;;
		*) echo "nl: bad option or missing option argument" >&2; exit 2 ;;
		esac
	fi
	shift $((OPTIND - 1))
	# parse-level faults
	case $M in
	b_a_as_t) [ "$bt" = a ] && bt=t ;;
	b_t_as_a) [ "$bt" = t ] && bt=a ;;
	b_n_as_t) [ "$bt" = n ] && bt=t ;;
	b_p_as_t) case $bt in p*) bt=t ;; esac ;;
	b_ignored) bt=t ;;
	h_ignored) ht=n ;;
	f_ignored) ft=n ;;
	h_as_f) pc_x=$ht ht=$ft ft=$pc_x ;;
	h_t_as_a) [ "$ht" = t ] && ht=a ;;
	h_a_as_t) [ "$ht" = a ] && ht=t ;;
	h_p_as_t) case $ht in p*) ht=t ;; esac ;;
	f_t_as_a) [ "$ft" = t ] && ft=a ;;
	f_a_as_t) [ "$ft" = a ] && ft=t ;;
	f_p_as_t) case $ft in p*) ft=t ;; esac ;;
	n_ln_right) [ "$fmt" = ln ] && fmt=rn ;;
	n_rn_zero) [ "$fmt" = rn ] && fmt=rz ;;
	n_rz_space) [ "$fmt" = rz ] && fmt=rn ;;
	n_ignored) fmt=rn ;;
	w_ignored) width=6 ;;
	w_plus1) width=$((width + 1)) ;;
	w_minus1) width=$((width - 1)) ;;
	w_ln_ignored) [ "$fmt" = ln ] && width=6 ;;
	w_rz_ignored) [ "$fmt" = rz ] && width=6 ;;
	s_ignored) sep=$(printf '\t') ;;
	s_first_only) sep=$(printf '%s' "$sep" | cut -c1) ;;
	v_ignored) start=1 ;;
	v_off1) start=$((start + 1)) ;;
	v_zero_default) [ "$start" = 0 ] && start=1 ;;
	i_ignored) incr=1 ;;
	i_off1) incr=$((incr + 1)) ;;
	vi_swapped) pc_x=$start start=$incr incr=$pc_x ;;
	l_ignored) lnum=1 ;;
	l_off1) lnum=$((lnum + 1)) ;;
	esac
	if [ -n "$dgiven" ]; then
		d1=$(printf '%s' "$d" | cut -c1)
		d2=$(printf '%s' "$d" | cut -c2)
		[ -n "$d2" ] || d2=:
		case $M in
		d_ignored) d1='\' d2=':' ;;
		d_swapped) pc_x=$d1 d1=$d2 d2=$pc_x ;;
		d_single_dup) [ ${#d} -eq 1 ] && d2=$d1 ;;
		d_single_bs) [ ${#d} -eq 1 ] && d2='\' ;;
		d_single_ignored) [ ${#d} -eq 1 ] && { d1='\' d2=':'; } ;;
		esac
	fi
	in=${1:-}
	if [ "$M" = stdin_unread ] && [ $# -eq 0 ]; then
		echo "nl: no input file" >&2
		exit 1
	fi
	[ "$M" = file_ignored ] && in=
	if [ -n "$in" ] && { [ ! -r "$in" ] || [ -d "$in" ]; }; then
		echo "nl: $in: cannot open" >&2
		[ "$M" = missing_status0 ] && exit 0
		exit 1
	fi
	if [ -n "$in" ]; then exec <"$in"; fi
	if [ "$M" = no_newline ]; then
		pc_o=$(PCN_M=$M PCN_BT=$bt PCN_HT=$ht PCN_FT=$ft PCN_INCR=$incr PCN_L=$lnum \
		    PCN_FMT=$fmt PCN_SEP=$sep PCN_START=$start PCN_W=$width \
		    PCN_D1=$d1 PCN_D2=$d2 PCN_P=$pflag PCN_DGIVEN=$dgiven \
		    awk -f "$PC_NLREF_DIR/nlref.awk")
		printf '%s' "$pc_o"
		exit 0
	fi
	PCN_M=$M PCN_BT=$bt PCN_HT=$ht PCN_FT=$ft PCN_INCR=$incr PCN_L=$lnum \
	    PCN_FMT=$fmt PCN_SEP=$sep PCN_START=$start PCN_W=$width \
	    PCN_D1=$d1 PCN_D2=$d2 PCN_P=$pflag PCN_DGIVEN=$dgiven \
	    awk -f "$PC_NLREF_DIR/nlref.awk"
	rc=$?
	[ "$M" = exit1 ] && rc=1
	exit "$rc"
)
