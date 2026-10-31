# the \cREc form of a context address is not recognized
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		'\'?*) echo "sed: unknown command" >&2; return 1 ;;
		esac
	done
	command sed "$@"
}
