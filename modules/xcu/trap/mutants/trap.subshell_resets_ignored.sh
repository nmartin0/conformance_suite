# a subshell restores the default for ignored signals
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# pcsub is replaced: the subshell first sets USR1 and USR2 back to the default action.
cat >pc-mut.part <<'PCM_EOF'
pcm_sed='s/^pcsub() .*$/pcsub() { pcm_sub "$@"; }/'
pcm_sub() {
	( command trap - USR1 USR2; "$@" )
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
