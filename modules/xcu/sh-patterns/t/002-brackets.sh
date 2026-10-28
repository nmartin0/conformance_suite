# Bracket expressions match one character from a list, a range or a class, !
# negates, a ] first and a - first or last are literal.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.sh-patterns.002
. "$PC_CORE/pc.sh"
pc_begin XCU.sh-patterns.002
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
pcyes b '[abc]' b '[a-c]' b '[!a]' x '[!a-c]' ']' '[]]' a '[]a]' - '[a-]' - '[-a]' 5 '[[:digit:]]' a '[[:alpha:]]' A '[[:upper:]]' a '[![:digit:]]' _ '[_[:alpha:]]' ac 'a[a-c]' 'ab' 'a[bc]'
pcno d '[abc]' a '[!a]' b '[!a-c]' a '[[:digit:]]' 5 '[![:digit:]]' ']' '[a]' B '[a-z]' a '[A-Z]' '' '[a]' ab '[ab]'
pc_pass
