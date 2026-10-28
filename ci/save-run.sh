#!/bin/sh
# usage: ci/save-run.sh SYSTEM-NAME ROOT-STDOUT [USER-STDOUT [SYSTEM-INFO-FILE]]
# Copies the outputs of a posixcheck run into reports/<date>-<SYSTEM-NAME>/.
set -eu
name=$1; root=$2; user=${3:-}; info=${4:-}
here=$(cd "$(dirname "$0")/.." && pwd)
dir=$here/reports/$(date +%Y-%m-%d)-$name
mkdir -p "$dir"
cp "$root" "$dir/root.txt"
[ -n "$user" ] && [ -f "$user" ] && cp "$user" "$dir/user.txt"
[ -n "$info" ] && [ -f "$info" ] && cp "$info" "$dir/system.txt"
{ for f in root user; do [ -f "$dir/$f.txt" ] && grep -E '^(FAIL|INSPECT|UNRESOLVED)' "$dir/$f.txt" | sed "s/^/$f /"; done; } >"$dir/not-pass.txt" || true
{ for f in root user; do [ -f "$dir/$f.txt" ] && grep -E '^SUMMARY' "$dir/$f.txt" | sed "s/^/$f /"; done; } >"$dir/summary.txt" || true
echo "saved $dir"
