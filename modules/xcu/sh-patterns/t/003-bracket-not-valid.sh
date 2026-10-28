# A [ that does not start a valid bracket expression matches a [ itself.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.003
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.003
# pcm STRING PATTERN: succeed when the pattern, with its special characters
# active, matches the string (case command).
pcm() {
	case $1 in
	$2) return 0 ;;
	esac
	return 1
}
# pcyes / pcno: the pairs STRING PATTERN that must / must not match.
pcyes() {
	while [ "$#" -ge 2 ]; do
		pcm "$1" "$2" || pc_fail "'$1' should match the pattern '$2'"
		shift 2
	done
}
pcno() {
	while [ "$#" -ge 2 ]; do
		pcm "$1" "$2" && pc_fail "'$1' should not match the pattern '$2'"
		shift 2
	done
}
pc_test
pcyes '[' '[' 'a[b' 'a[b' '[a' '[a'
pcno a '[a' '' '['
pc_pass
