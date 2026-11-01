# Shared by the m4 faults and variants: a small reimplementation of m4 with
# one planted fault or one conforming variant, chosen by name (see m4ref.awk).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# pc_m4ref KIND NAME ARGS...: behaves as m4 does, except for the fault (KIND
# F) or with the conforming choice (KIND V).
pc_m4ref_awk=${PC_MUTANT_FILE%/*}/m4ref.awk
[ -f "$pc_m4ref_awk" ] || pc_m4ref_awk=${PC_MUTANT_FILE%/*}/../mutants/m4ref.awk
pc_m4ref() (
	pc_k=$1 pc_n=$2
	shift 2
	pc_ops=pcm4-ops.$$
	: >"$pc_ops"
	trap 'rm -f "$pc_ops"' EXIT
	case $pc_k in F) pc_f=$pc_n pc_v= ;; *) pc_f= pc_v=$pc_n ;; esac
	pc_opts=1
	while [ $# -gt 0 ]; do
		if [ $pc_opts = 1 ]; then
			case $1 in
			--) pc_opts=0; shift; [ "$pc_f" = dashdash_as_file ] && echo "F --" >>"$pc_ops"; continue ;;
			-s) echo "S x" >>"$pc_ops"; shift; continue ;;
			-D) echo "D $2" >>"$pc_ops"; shift 2; continue ;;
			-U) echo "U $2" >>"$pc_ops"; shift 2; continue ;;
			-D?*) echo "D ${1#-D}" >>"$pc_ops"; shift; continue ;;
			-U?*) echo "U ${1#-U}" >>"$pc_ops"; shift; continue ;;
			-) ;;
			-?*) echo "m4: unknown option $1" >&2; exit 1 ;;
			esac
			[ "$pc_f" = options_stop_at_operand ] && pc_opts=0
		fi
		echo "F $1" >>"$pc_ops"
		shift
	done
	awk -v F="$pc_f" -v V="$pc_v" -v OPS="$pc_ops" -v PID="$$" -f "$pc_m4ref_awk"
)
