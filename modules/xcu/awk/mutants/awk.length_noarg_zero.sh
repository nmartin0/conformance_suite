# awk length without an argument is 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone awk, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v awk)" >pc-mutbin/awk
cat >>pc-mutbin/awk <<'PC_EOF'
q() { if [ -z "$1" ]; then printf "''"; else printf "%s" "$1" | sed "s/'/'\\\\''/g;1s/^/'/;\$s/\$/'/"; fi; }
add() { acc="$acc $(q "$1")"; }
hook_F() { add -F; add "$1"; }
hook_v() { add -v; add "$1"; }
hook_f() { add -f; add "$1"; }
hook_prog() { add "$1"; }
hook_arg() { add "$1"; }
hook_opt() { add "$1"; }
hook_dd() { add --; }
run() { exec "$REAL" "$@"; }
hook_prog() { p=$(printf '%s\n' "$1" | sed -E 's/length\(\)/0/g;s/length([^(A-Za-z0-9_])/0\1/g')
	add "$p"; }
acc= nf=0
while [ $# -gt 0 ]; do
	case $1 in
	--) hook_dd; shift; break ;;
	-F) att=0; hook_F "$2"; shift 2 ;;
	-F?*) att=1; hook_F "${1#-F}"; shift ;;
	-v) att=0; hook_v "$2"; shift 2 ;;
	-v?*) att=1; hook_v "${1#-v}"; shift ;;
	-f) att=0; nf=$((nf + 1)); hook_f "$2"; shift 2 ;;
	-f?*) att=1; nf=$((nf + 1)); hook_f "${1#-f}"; shift ;;
	-?*) hook_opt "$1"; shift ;;
	*) break ;;
	esac
done
if [ $nf = 0 ] && [ $# -gt 0 ]; then hook_prog "$1"; shift; fi
for a; do hook_arg "$a"; done
eval "set -- $acc"
run "$@"
PC_EOF
chmod +x pc-mutbin/awk
PATH=$PWD/pc-mutbin:$PATH
