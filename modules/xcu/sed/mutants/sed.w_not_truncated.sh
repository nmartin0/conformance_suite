# an existing w file keeps its old contents; new lines are added after them
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
pcm_wnames() {
	for pcm_a
	do
		printf '%s\n' "$pcm_a" | command sed -n -E 's/^.*(^|[[:blank:]\/])w[[:blank:]]+([^[:blank:]]+)$/\2/p'
	done
}
sed() {
	pcm_i=0
	for pcm_f in $(pcm_wnames "$@")
	do
		pcm_i=$((pcm_i + 1))
		if [ -f "$pcm_f" ]; then cp "$pcm_f" "pc-mut-old.$$.$pcm_i"; fi
	done
	command sed "$@"
	pcm_rc=$?
	pcm_i=0
	for pcm_f in $(pcm_wnames "$@")
	do
		pcm_i=$((pcm_i + 1))
		if [ -f "pc-mut-old.$$.$pcm_i" ]; then
			cat "pc-mut-old.$$.$pcm_i" "$pcm_f" >"pc-mut-new.$$" && mv "pc-mut-new.$$" "$pcm_f"
			rm -f "pc-mut-old.$$.$pcm_i"
		fi
	done
	return $pcm_rc
}
