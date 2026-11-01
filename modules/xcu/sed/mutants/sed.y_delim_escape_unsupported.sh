# an escaped delimiter inside y is not recognized
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		'y/'*'\/'*) echo "sed: strings for y command are different lengths" >&2; return 1 ;;
		esac
	done
	command sed "$@"
}
