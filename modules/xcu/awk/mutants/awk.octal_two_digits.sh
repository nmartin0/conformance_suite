# awk an octal escape in a string constant takes at most two digits
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
hook_prog() { p=$(printf '%s\n' "$1" | "$REAL" '{ o = ""; s = 0; n = length($0); for (i = 1; i <= n; i++) { c = substr($0, i, 1); if (s && c == "\\") { d = substr($0, i + 1, 3); if (d ~ /^[0-7][0-7][0-7]$/) { o = o "\\" substr(d, 1, 2) "\" \"" substr(d, 3, 1); i += 3 } else { o = o c substr($0, i + 1, 1); i++ } } else { if (c == "\"") s = !s; o = o c } } print o }'); add "$p"; }
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
