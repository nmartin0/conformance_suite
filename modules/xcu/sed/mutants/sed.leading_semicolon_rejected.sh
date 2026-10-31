# a script starting with a blank or a semicolon is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() {
	for pcm_a
	do
		case $pcm_a in
		' '* | ';'*) echo "sed: bad script" >&2; return 1 ;;
		esac
	done
	command sed "$@"
}
