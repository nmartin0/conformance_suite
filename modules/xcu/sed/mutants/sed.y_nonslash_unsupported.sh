# a delimiter other than slash is not accepted by y
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		y/*) ;;
		y[!a-zA-Z0-9\ ]*) echo "sed: unterminated y command" >&2; return 1 ;;
		esac
	done
	command sed "$@"
}
