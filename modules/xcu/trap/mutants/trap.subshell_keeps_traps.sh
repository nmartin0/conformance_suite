# a subshell keeps the caught traps of its parent
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# pcsub is replaced: the subshell first reads the parent's trap listing back.
cat >pc-mut.part <<'PCM_EOF'
pcm_sed='s/^pcsub() .*$/pcsub() { pcm_sub "$@"; }/'
pcm_sub() {
	command trap >"$pcm_dir/pc-mut.sl"
	( . "$pcm_dir/pc-mut.sl"; "$@" )
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
