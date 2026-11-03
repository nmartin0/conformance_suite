#!/bin/sh
# usage: ci/save-run.sh SYSTEM-NAME ROOT-STDOUT [USER-STDOUT [SYSTEM-INFO-FILE
#                       [ROOT-RESULTS-DIR [USER-RESULTS-DIR]]]]
# The results directories (the --results DIR of each run) keep run.log, which
# analysis/analyze.py needs; stdout alone cannot be loaded or calibrated.
# Copies the outputs of a posixcheck run into reports/<date>-<SYSTEM-NAME>/.
set -eu
name=$1; root=$2; user=${3:-}; info=${4:-}; rootdir=${5:-}; userdir=${6:-}
here=$(cd "$(dirname "$0")/.." && pwd)
dir=$here/reports/$(date +%Y-%m-%d)-$name
mkdir -p "$dir"
cp "$root" "$dir/root.txt"
[ -n "$user" ] && [ -f "$user" ] && cp "$user" "$dir/user.txt"
[ -n "$info" ] && [ -f "$info" ] && cp "$info" "$dir/system.txt"
if [ -n "$rootdir" ]; then
	[ -f "$rootdir/run.log" ] || { echo "no run.log in $rootdir" >&2; exit 1; }
	cp "$rootdir/run.log" "$dir/root.run.log"
fi
if [ -n "$userdir" ]; then
	[ -f "$userdir/run.log" ] || { echo "no run.log in $userdir" >&2; exit 1; }
	cp "$userdir/run.log" "$dir/user.run.log"
fi
{ for f in root user; do [ -f "$dir/$f.txt" ] && grep -E '^(FAIL|INSPECT|UNRESOLVED)' "$dir/$f.txt" | sed "s/^/$f /"; done; } >"$dir/not-pass.txt" || true
{ for f in root user; do [ -f "$dir/$f.txt" ] && grep -E '^SUMMARY' "$dir/$f.txt" | sed "s/^/$f /"; done; } >"$dir/summary.txt" || true
echo "saved $dir"
