# a conforming shell that quotes listed actions with double quotes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text fixes the format as trap -- action conditions and asks only that the quoting be right for reinput (XCU 2.15 trap, DESCRIPTION).
# This system quotes with double quotes; it is limited to actions without special characters, where that is correct quoting.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list|p)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		sed "s/^trap -- '\([A-Za-z0-9 ;]*\)' /trap -- \"\1\" /" "$pcm_dir/pc-mut.l1"; return 0 ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
