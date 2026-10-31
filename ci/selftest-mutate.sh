#!/bin/sh
# selftest-mutate.sh - ci/mutate.sh must give the known answers on its fixture.
#
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# usage: ci/selftest-mutate.sh [mutate.sh options, such as --cc musl-gcc, --cache, -j 2]
#
# ci/selftest/modules is a tree of fixture modules (not part of the suite)
# whose faults and variants each force one outcome: CAUGHT, MISSED,
# FLAKY, TOLERATED, REJECTED, UNDECIDED, READING, UNCHECKED, SYSTEM (a
# build shim's run that fails), and a fault planted on a header-only variant;
# ci/selftest/shells checks --sut-sh the same way. Any change to ci/mutate.sh or to the
# driver must leave this check passing before it is used.

set -u
here=$(cd "$(dirname "$0")/.." && pwd)
n=0
while :; do
	t=${TMPDIR:-/tmp}/pcst.$$.$n
	mkdir -m 700 "$t" 2>/dev/null && break
	n=$((n + 1))
	[ "$n" -lt 100 ] || { echo "selftest-mutate: cannot create a work directory" >&2; exit 2; }
done
trap 'rm -rf "$t"' EXIT
printf '0' >"$t/flip"
PC_FX_FLIP=$t/flip
export PC_FX_FLIP
# PC_MUTATE_SH: check a candidate replacement before it is moved into place.
sh "${PC_MUTATE_SH:-$here/ci/mutate.sh}" --root "$here/ci/selftest/modules" ${1+"$@"} >"$t/out" 2>&1
rc=$?
awk '/^(CAUGHT|MISSED|FLAKY|TOLERATED|REJECTED|UNDECIDED|READING|UNCHECKED|SYSTEM) / { print $1, $2, $3 }' "$t/out" >"$t/got"
if ! cmp -s "$t/got" "$here/ci/selftest/expected.txt"; then
	echo "selftest-mutate: FAILED: ci/mutate.sh did not give the known answers" >&2
	diff "$here/ci/selftest/expected.txt" "$t/got" >&2
	cat "$t/out" >&2
	exit 1
fi
if [ "$rc" -eq 0 ]; then
	echo "selftest-mutate: FAILED: ci/mutate.sh exited 0 with MISSED, REJECTED and FLAKY results" >&2
	exit 1
fi
nok=$(wc -l <"$t/got" | tr -d ' ')

# --sut-sh: ci/selftest/shells has shell tests whose baselines differ between
# two wrapper shells (shA, shB; both run sh). Under shA alone 003 is caught
# and 002 cannot be judged; under both, CAUGHT needs one judging shell and
# MISSED any one.
S=$here/ci/selftest/shells
for pair in "sutsh-a:--sut-sh $S/shA" "sutsh-ab:--sut-sh $S/shA --sut-sh $S/shB"; do
	name=${pair%%:*}
	# shellcheck disable=SC2086 # the options are a word list
	sh "${PC_MUTATE_SH:-$here/ci/mutate.sh}" --root "$S" ${pair#*:} ${1+"$@"} >"$t/out" 2>&1
	rc=$?
	awk '/^(CAUGHT|MISSED|FLAKY|TOLERATED|REJECTED|UNDECIDED|READING|UNCHECKED|SYSTEM) / { print $1, $2, $3 }' "$t/out" >"$t/got"
	if ! cmp -s "$t/got" "$here/ci/selftest/expected-$name.txt"; then
		echo "selftest-mutate: FAILED: ci/mutate.sh --sut-sh ($name) did not give the known answers" >&2
		diff "$here/ci/selftest/expected-$name.txt" "$t/got" >&2
		cat "$t/out" >&2
		exit 1
	fi
	if [ "$name" = sutsh-ab ] && [ "$rc" -eq 0 ]; then
		echo "selftest-mutate: FAILED: ci/mutate.sh --sut-sh exited 0 with a MISSED result" >&2
		exit 1
	fi
done
echo "selftest-mutate: ok ($nok known answers, and 8 under --sut-sh)"
