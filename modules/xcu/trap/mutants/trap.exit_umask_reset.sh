# the EXIT action runs with the file mode mask of the trap's installation
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The file mode creation mask is put back to its value at installation before an EXIT action runs.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		case ${1-} in
		EXIT|0) pcm_u0=$(umask); command trap -- "umask \"\$pcm_u0\"${pcm_nl}$pcm_act" "$@" ;;
		*) command trap -- "$pcm_act" "$@" ;;
		esac ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
