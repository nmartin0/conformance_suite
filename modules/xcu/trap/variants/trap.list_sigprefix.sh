# a conforming shell that spells conditions with the SIG prefix
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text lets an implementation accept names with the SIG prefix as an extension; such a shell may list them that way too, since the listing only has to read back as trap commands (XCU 2.15 trap, DESCRIPTION).
# It accepts the prefix on input and writes it on output for the one-condition lines the tests make.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list|p)
		if [ "$pcm_kind" = p ]; then shift; fi
		for pcm_c; do case $pcm_c in SIG*) pcm_c=${pcm_c#SIG} ;; esac; set -- "$@" "$pcm_c"; shift; done
		if [ "$pcm_kind" = p ]; then command trap -p "$@" >"$pcm_dir/pc-mut.l1" || return $?; else command trap >"$pcm_dir/pc-mut.l1" || return $?; fi
		sed "/ EXIT\$/!s/^\\(trap -- .*' \\)\\(SIG\\)\\{0,1\\}\\([A-Z][A-Z0-9]*\\)\$/\\1SIG\\3/" "$pcm_dir/pc-mut.l1"; return 0 ;;
	set|null|hyphen)
		shift "$pcm_n"
		for pcm_c; do case $pcm_c in SIG*) pcm_c=${pcm_c#SIG} ;; esac; set -- "$@" "$pcm_c"; shift; done
		command trap -- "$pcm_act" "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
