# after a trap action $? is the status of the action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# pcfg is replaced: after the foreground command, if a trap action ran, its status is returned.
cat >pc-mut.part <<'PCM_EOF'
pcm_sed='s/^pcfg() .*$/pcfg() { pcm_fg "$@"; }/'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; command trap -- "$pcm_act${pcm_nl}pcm_ast=\$?; pcm_ran=1" "$@" ;;
	*) command trap "$@" ;;
	esac
}
pcm_fg() {
	${PC_SH:-sh} "$@"
	pcm_s=$?
	if [ -n "${pcm_ran-}" ]; then pcm_ran=; return "$pcm_ast"; fi
	return "$pcm_s"
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
