# standard input is not read when no file operand is a regular file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	pcm_f=0
	for pcm_a
	do
		if [ -f "$pcm_a" ]; then pcm_f=1; fi
	done
	if [ "$pcm_f" = 0 ]; then command sed "$@" </dev/null; else command sed "$@"; fi
}
