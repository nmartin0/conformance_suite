# the EXIT action runs in the directory the shell started in
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Before an EXIT action runs the working directory is set back to the one when the trap was installed.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set)
		shift "$pcm_n"
		case ${1-} in
		EXIT|0) pcm_cwd0=$PWD; command trap -- "cd \"\$pcm_cwd0\"${pcm_nl}$pcm_act" "$@" ;;
		*) command trap -- "$pcm_act" "$@" ;;
		esac ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
