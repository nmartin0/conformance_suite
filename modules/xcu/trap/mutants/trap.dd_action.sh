# trap -- takes -- as the action
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The word -- is read as the action and the rest as conditions.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	if [ -n "$pcm_dd" ]; then shift; command trap "-- " "$@"; return $?; fi
	command trap "$@"
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
