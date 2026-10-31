# the i flag of s is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		s*/i | s*/ig | s*/gi) echo "sed: unknown option to s" >&2; return 1 ;;
		esac
	done
	command sed "$@"
}
