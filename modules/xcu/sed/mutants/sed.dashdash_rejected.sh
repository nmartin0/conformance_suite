# the -- argument is not taken as the end of the options
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		--) echo "sed: unrecognized option '--'" >&2; return 1 ;;
		esac
	done
	command sed "$@"
}
