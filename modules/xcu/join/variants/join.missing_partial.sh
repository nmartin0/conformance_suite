# a conforming join that writes what it can before it reports an unreadable operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because the text says only that an error status is returned when
# an error occurs; it does not say what is written to standard output
# before the failure is found (XCU join EXIT STATUS, STDOUT). The unreadable
# operand is treated as empty for the output, then the error is reported.
join() (
	pc_n=$#
	pc_i=0
	pc_bad=0
	while [ "$pc_i" -lt "$pc_n" ]; do
		pc_a=$1
		shift
		pc_i=$((pc_i + 1))
		if [ "$pc_i" -gt $((pc_n - 2)) ] && [ "$pc_a" != - ] && [ ! -r "$pc_a" ]; then
			pc_a=/dev/null
			pc_bad=1
		fi
		set -- "$@" "$pc_a"
	done
	command join "$@"
	pc_rc=$?
	if [ "$pc_bad" = 1 ]; then
		echo "join: an operand could not be read" >&2
		return 1
	fi
	return "$pc_rc"
)
