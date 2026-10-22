#!/bin/sh
# mutate.sh - check that every test catches the faults it declares (CI).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: ci/mutate.sh [--sh SHELL] [ID-pattern]
#
# Runs off-system on a Linux or BSD machine with LD_PRELOAD. For each test
# that declares MUTANT lines (ci/lint.py --list-mutants):
#   1. run the test normally (the baseline)
#   2. run it once per mutant, with the planted fault active
# A mutant is CAUGHT when the baseline is PASS and the mutant run is FAIL.
# Anything else is reported; MISSED (the test passed with the fault in
# place) makes the script exit 1. When the baseline is not PASS on this
# machine, the mutant cannot be judged here and is reported UNCHECKED.

set -u
here=$(cd "$(dirname "$0")/.." && pwd)
sh=sh
pattern='*'
while [ $# -gt 0 ]; do
	case $1 in
	--sh) sh=$2; shift 2 ;;
	*) pattern=$1; shift ;;
	esac
done

work=$(mktemp -d "${TMPDIR:-/tmp}/pcmutate.XXXXXX")
trap 'rm -rf "$work"' EXIT

code_of() { # results-dir id
	if [ -f "$1/state/$2" ]; then cat "$1/state/$2"; else echo NONE; fi
}

note_of() { # results-dir id: the note logged for id
	awk -F '\t' -v id="$2" '$2 == id { print $4 }' "$1/run.log" 2>/dev/null
}

run() { # results-dir id [mutant]
	if [ $# -eq 3 ]; then
		"$sh" "$here/posixcheck.sh" --no-selftest --results "$1" \
		    --only "$2" --mutant "$3" >/dev/null 2>&1
	else
		"$sh" "$here/posixcheck.sh" --no-selftest --results "$1" \
		    --only "$2" >/dev/null 2>&1
	fi
}

caught=0 missed=0 unchecked=0 n=0 last=
python3 "$here/ci/lint.py" --list-mutants >"$work/pairs" || exit 2
while read -r id mutant; do
	case $id in $pattern) ;; *) continue ;; esac
	n=$((n + 1))
	if [ "$id" != "$last" ]; then
		run "$work/base.$n" "$id"
		base=$(code_of "$work/base.$n" "$id")
		last=$id
	fi
	if [ "$base" != PASS ]; then
		printf 'UNCHECKED %-40s %-28s baseline %s\n' "$id" "$mutant" "$base"
		unchecked=$((unchecked + 1))
		continue
	fi
	run "$work/m.$n" "$id" "$mutant"
	got=$(code_of "$work/m.$n" "$id")
	if [ "$got" = FAIL ]; then
		printf 'CAUGHT    %-40s %-28s %s\n' "$id" "$mutant" \
		    "$(note_of "$work/m.$n" "$id")"
		caught=$((caught + 1))
	else
		printf 'MISSED    %-40s %-28s mutant run gave %s\n' "$id" "$mutant" "$got"
		missed=$((missed + 1))
	fi
done <"$work/pairs"

printf '\nmutate: %d mutants: %d caught, %d missed, %d unchecked\n' \
    "$n" "$caught" "$missed" "$unchecked"
[ "$missed" -eq 0 ]
