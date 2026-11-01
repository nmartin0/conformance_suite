# the listing names another signal for one ignored on entry
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# The line for a condition recorded as ignored by the parent names USR2 instead.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		sed 's/USR1/USR2/' "$pcm_dir/pc-mut.l1"; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
