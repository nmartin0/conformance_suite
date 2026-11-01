# a trap runs as soon as the signal arrives, not after the foreground command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# pcfg is replaced: the foreground command is run in the background and waited for, and wait returns early when a trap fires.
cat >pc-mut.part <<'PCM_EOF'
pcm_sed='s/^pcfg() .*$/pcfg() { pcm_fg "$@"; }/'
pcm_fg() {
	${PC_SH:-sh} "$@" &
	pcm_pid=$!
	wait "$pcm_pid"
	wait "$pcm_pid"
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
