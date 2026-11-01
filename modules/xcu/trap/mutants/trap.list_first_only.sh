# the listing has only its first line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Only the first line of the listing is written.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		sed -n 1p "$pcm_dir/pc-mut.l1"; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
