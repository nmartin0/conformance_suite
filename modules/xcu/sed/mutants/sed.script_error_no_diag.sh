# no diagnostic is written for a script that cannot be used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pcm_class() {
	pcm_k=script
	pcm_have=0
	pcm_skip=
	for pcm_a
	do
		if [ -n "$pcm_skip" ]; then
			if [ "$pcm_skip" = f ] && [ ! -e "$pcm_a" ]; then pcm_k=scriptfile; return; fi
			pcm_skip=
			continue
		fi
		case $pcm_a in
		-e) pcm_have=1; pcm_skip=e ;;
		-f) pcm_have=1; pcm_skip=f ;;
		-*) ;;
		*)
			if [ "$pcm_have" = 0 ]; then pcm_have=1; elif [ ! -e "$pcm_a" ]; then pcm_k=file; fi
			;;
		esac
	done
}
sed() {
	pcm_e=pc-mut-e.$$
	command sed "$@" 2>"$pcm_e"
	pcm_rc=$?
	pcm_class "$@"
	if [ "$pcm_rc" -eq 0 ] || [ "$pcm_k" != script ]; then cat "$pcm_e" >&2; fi
	rm -f "$pcm_e"
	return $pcm_rc
}
