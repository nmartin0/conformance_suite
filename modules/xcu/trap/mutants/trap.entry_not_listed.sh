# the listing leaves out a signal that was ignored on entry
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Conditions recorded as ignored by the parent and not set since are dropped from the listing.
cat >pc-mut.part <<'PCM_EOF'
pcm_trap() {
	pcm_split "$@"
	case $pcm_kind in
	list)
		command trap "$@" >"$pcm_dir/pc-mut.l1" || return $?
		for pcm_c in ${PCM_IGN-}; do
			grep -v "$pcm_c\$" "$pcm_dir/pc-mut.l1" >"$pcm_dir/pc-mut.l2"
			cat "$pcm_dir/pc-mut.l2" >"$pcm_dir/pc-mut.l1"
		done
		cat "$pcm_dir/pc-mut.l1"; return 0 ;;
	null)
		shift "$pcm_n"
		for pcm_c; do PCM_IGN="${PCM_IGN-} $pcm_c"; done
		export PCM_IGN
		command trap -- '' "$@" ;;
	*) command trap "$@" ;;
	esac
}
PCM_EOF
. "${PC_MUTANT_FILE%/*/*}/mutants/trap-wrap.inc"
