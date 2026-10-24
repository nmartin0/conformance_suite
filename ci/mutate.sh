#!/bin/sh
# mutate.sh - check that every test catches the faults it declares (CI).
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: ci/mutate.sh [--sh SHELL] [--cc CC] [--profile FILE]
#                     [--faults-only | --variants-only] [ID-pattern]
#
# Runs off-system on a Linux or BSD machine with LD_PRELOAD. For each test
# that declares MUTANT lines (ci/lint.py --list-mutants):
#   1. run the test normally (the baseline)
#   2. run it once per mutant, with the planted fault active
# A mutant is CAUGHT when the baseline is PASS and the mutant run is FAIL
# (or INSPECT, a disputed assertion's FAIL).
# Anything else is reported; MISSED (the test passed with the fault in
# place) makes the script exit 1. When the baseline is not PASS on this
# machine, the mutant cannot be judged here and is reported UNCHECKED; so
# is a mutant whose shim reports "pcmutant: cannot plant" in the test's
# output (the fault needs something this system lacks, such as /proc).
#
# Then the other direction. For each VARIANT a test declares (behaviour the
# standard allows that the reference systems may not show, planted the same
# way), the test must still PASS:
#   TOLERATED  the variant run is PASS
#   REJECTED   the variant run is FAIL or INSPECT: the test asks for more
#              than the text does (exit 1)
#   UNDECIDED  any other result (UNRESOLVED and the like): the test cannot
#              judge a conforming system that behaves this way; reported,
#              not fatal, but each one needs a look
#   UNCHECKED  as for faults
# A rejected variant is a bug in the test, or the variant does not model
# what the text allows; it is never resolved by deleting the variant
# because a reference system does otherwise.

set -u
here=$(cd "$(dirname "$0")/.." && pwd)
sh=sh
cc=
profile=
pattern='*'
do_faults=yes do_variants=yes
while [ $# -gt 0 ]; do
	case $1 in
	--sh) sh=$2; shift 2 ;;
	--cc) cc=$2; shift 2 ;;
	--profile) profile=$2; shift 2 ;;
	--faults-only) do_variants=no; shift ;;
	--variants-only) do_faults=no; shift ;;
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

run() { # results-dir id [mutant|variant name]
	set -- "$@"
	r=$1 id=$2
	if [ $# -eq 4 ]; then set -- "$3" "$4"; else set --; fi
	if [ -n "$cc" ]; then set -- "$@" --cc "$cc"; fi
	if [ -n "$profile" ]; then set -- "$@" --profile "$profile"; fi
	"$sh" "$here/posixcheck.sh" --no-selftest --results "$r" --only "$id" \
	    "$@" >/dev/null 2>&1
}

caught=0 missed=0 unchecked=0 n=0 last=
python3 "$here/ci/lint.py" --list-mutants >"$work/pairs" || exit 2
[ "$do_faults" = yes ] || : >"$work/pairs"
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
	run "$work/m.$n" "$id" --mutant "$mutant"
	got=$(code_of "$work/m.$n" "$id")
	if [ "$got" = INSPECT ]; then
		case $(note_of "$work/m.$n" "$id") in disputed*) ;; *) got="INSPECT (not disputed)" ;; esac
	fi
	if [ "$got" != FAIL ] && [ "$got" != INSPECT ] &&
	    grep -q 'pcmutant: cannot plant' "$work/m.$n/out/$id.txt" 2>/dev/null; then
		printf 'UNCHECKED %-40s %-28s fault cannot be planted here\n' "$id" "$mutant"
		unchecked=$((unchecked + 1))
		continue
	fi
	# A disputed assertion logs its FAIL as INSPECT; that counts too.
	if [ "$got" = FAIL ] || [ "$got" = INSPECT ]; then
		printf 'CAUGHT    %-40s %-28s %s\n' "$id" "$mutant" \
		    "$(note_of "$work/m.$n" "$id")"
		caught=$((caught + 1))
	else
		printf 'MISSED    %-40s %-28s mutant run gave %s\n' "$id" "$mutant" "$got"
		missed=$((missed + 1))
	fi
done <"$work/pairs"

tolerated=0 rejected=0 undecided=0 vunchecked=0 nv=0
python3 "$here/ci/lint.py" --list-variants >"$work/vpairs" || exit 2
[ "$do_variants" = yes ] || : >"$work/vpairs"
while read -r id variant; do
	case $id in $pattern) ;; *) continue ;; esac
	nv=$((nv + 1)) n=$((n + 1))
	if [ "$id" != "$last" ]; then
		run "$work/base.$n" "$id"
		base=$(code_of "$work/base.$n" "$id")
		last=$id
	fi
	if [ "$base" != PASS ]; then
		printf 'UNCHECKED %-40s %-28s baseline %s\n' "$id" "$variant" "$base"
		vunchecked=$((vunchecked + 1))
		continue
	fi
	run "$work/v.$n" "$id" --variant "$variant"
	got=$(code_of "$work/v.$n" "$id")
	if [ "$got" != PASS ] && grep -q 'pcmutant: cannot plant' "$work/v.$n/out/$id.txt" 2>/dev/null; then
		printf 'UNCHECKED %-40s %-28s variant cannot be planted here\n' "$id" "$variant"
		vunchecked=$((vunchecked + 1))
		continue
	fi
	case $got in
	PASS)
		printf 'TOLERATED %-40s %-28s\n' "$id" "$variant"
		tolerated=$((tolerated + 1)) ;;
	FAIL | INSPECT)
		printf 'REJECTED  %-40s %-28s %s: %s\n' "$id" "$variant" "$got" "$(note_of "$work/v.$n" "$id")"
		rejected=$((rejected + 1)) ;;
	*)
		printf 'UNDECIDED %-40s %-28s %s: %s\n' "$id" "$variant" "$got" "$(note_of "$work/v.$n" "$id")"
		undecided=$((undecided + 1)) ;;
	esac
done <"$work/vpairs"

nm=$((n - nv))
printf '\nmutate: %d mutants: %d caught, %d missed, %d unchecked\n' \
    "$nm" "$caught" "$missed" "$unchecked"
printf 'mutate: %d variants: %d tolerated, %d rejected, %d undecided, %d unchecked\n' \
    "$nv" "$tolerated" "$rejected" "$undecided" "$vunchecked"
[ "$missed" -eq 0 ] && [ "$rejected" -eq 0 ]
