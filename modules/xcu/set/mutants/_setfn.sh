# Prelude text for the faults of "set operands": pcm_set prints the
# command that gives the positional parameters their (wrong) new values.
# Used as: eval "$(pcm_set OLDCOUNT OLD... :: NEW...)"
pcm_q() {
	printf "'"
	printf '%s' "$1" | sed "s/'/'\\\\''/g"
	printf "'"
}
pcm_set() {
	pcm_n=$1
	shift
	pcm_i=1
	while [ "$pcm_i" -le "$pcm_n" ]; do
		eval "pcm_o$pcm_i=\$(pcm_q \"\$1\")"
		shift
		pcm_i=$((pcm_i + 1))
	done
	shift
	pcm_k=$#
	pcm_j=0
	pcm_new=
	for pcm_a do
		pcm_j=$((pcm_j + 1))
		case $PCM_MODE in
		droplast) [ "$pcm_j" -eq "$pcm_k" ] && continue ;;
		dropfirst) [ "$pcm_j" -eq 1 ] && continue ;;
		dropempty) [ -z "$pcm_a" ] && continue ;;
		max9) [ "$pcm_j" -gt 9 ] && continue ;;
		esac
		if [ "$PCM_MODE" = reverse ]; then
			pcm_new=" $(pcm_q "$pcm_a")$pcm_new"
		else
			pcm_new="$pcm_new $(pcm_q "$pcm_a")"
		fi
	done
	case $PCM_MODE in
	append)
		pcm_all=
		pcm_i=1
		while [ "$pcm_i" -le "$pcm_n" ]; do
			eval "pcm_all=\"\$pcm_all \$pcm_o$pcm_i\""
			pcm_i=$((pcm_i + 1))
		done
		pcm_new="$pcm_all$pcm_new"
		;;
	keepold)
		pcm_i=$((pcm_k + 1))
		while [ "$pcm_i" -le "$pcm_n" ]; do
			eval "pcm_new=\"\$pcm_new \$pcm_o$pcm_i\""
			pcm_i=$((pcm_i + 1))
		done
		;;
	esac
	printf 'set --%s\n' "$pcm_new"
}
