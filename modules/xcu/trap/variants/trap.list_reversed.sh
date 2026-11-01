# a conforming shell that lists conditions in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text does not fix the order of the lines in a listing (XCU 2.15 trap, DESCRIPTION).
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list|p)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		sed '1!G;h;$!d' "$pcm_dir/pc-mut.l1"; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
