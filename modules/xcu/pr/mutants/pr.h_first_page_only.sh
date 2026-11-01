# pr -h replaces the name on the first page only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
H= prev= last=
for a
do
	[ "$prev" = -h ] && H=$a
	prev=$a last=$a
done
[ -n "$H" ] || exec "$REAL" "$@"
"$REAL" "$@" | awk -v h="$H" -v f="$last" '{ if (match($0, / Page [0-9]+$/)) { n = substr($0, RSTART + 6) + 0; if (n > 1) { i = index($0, " " h " Page "); if (i > 0) $0 = substr($0, 1, i) f substr($0, i + 1 + length(h)) } } print }'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
