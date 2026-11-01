# Runs the reimplementation: the fault or variant is named by PC_PF_FAULT.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
here=${0%/*}
n=$#
while [ "$n" -gt 0 ]; do
	set -- "$@" "@$1"
	shift
	n=$((n - 1))
done
loc=C
if [ -n "${LC_ALL:-}" ]; then
	loc=$LC_ALL
elif [ -n "${LC_NUMERIC:-}" ]; then
	loc=$LC_NUMERIC
elif [ -n "${LANG:-}" ]; then
	loc=$LANG
fi
LC_ALL=C
export LC_ALL
exec awk -v M="${PC_PF_FAULT:-none}" -v LOC="$loc" -f "$here/printfref.awk" -- "$@"
