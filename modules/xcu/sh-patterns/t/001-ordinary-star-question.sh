# Ordinary characters match themselves, ? matches exactly one character, *
# matches any string including the empty one (each * as much as lets the
# rest match).
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.001
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.001
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
pcyes abc abc abc 'a?c' abc '???' abc '*' abc 'a*' abc '*c' abc 'a*c' abc '*b*' ac 'a*c' abcbc 'a*bc' '' '*' aXbXc 'a*b*c' abc 'a**c' ac 'a**c' 'a b' 'a?b' abab '*ab' abab 'ab*' abab '*b' abab '*a*b'
pcno abc ab abc '????' abc 'a?' ab '?' '' '?' abc 'b*' abc '*b' '' a abab '*a' abab 'b*' abc 'a*d'
pc_pass
