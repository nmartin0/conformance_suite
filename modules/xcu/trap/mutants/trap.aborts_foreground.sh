# a trap kills the foreground command at once
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# pcfg is replaced as for runs_now, and the action first ends the foreground command.
cat >pc-mut.part <<'PCM_EOF'
pcm_sed='s/^pcfg() .*$/pcfg() { pcm_fg "$@"; }/'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	set) shift "$pcm_n"; command trap -- "$pcm_act${pcm_nl}command kill \"\$pcm_pid\" 2>/dev/null" "$@" ;;
	*) command trap "$@" ;;
	esac
}
pcm_fg() {
	${PC_SH:-sh} "$@" &
	pcm_pid=$!
	wait "$pcm_pid"
	wait "$pcm_pid"
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
