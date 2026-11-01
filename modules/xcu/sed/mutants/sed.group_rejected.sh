# options grouped behind one dash are an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		-[a-zA-Z][a-zA-Z]*) echo "sed: invalid option -- '${pcm_a#-?}'" >&2; return 1 ;;
		esac
	done
	command sed "$@"
}
