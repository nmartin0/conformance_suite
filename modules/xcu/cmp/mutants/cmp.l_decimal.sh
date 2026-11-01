# -l writes the differing bytes in decimal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cmp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cmp)" >pc-mutbin/cmp
cat >>pc-mutbin/cmp <<'PC_EOF'
T=${TMPDIR:-/tmp}/pcmutcmp.$$
trap 'rm -f "$T".o "$T".e "$T".a "$T".b "$T".p' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"
rc=$?
if [ $rc -eq 1 ]; then
while read -r n x y; do
printf "%s %d %d\n" "$n" "$((0$x))" "$((0$y))"
done <"$T.o" >"$T.p"
cat "$T.p" >"$T.o"
fi
cat "$T.o"
cat "$T.e" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/cmp
PATH=$PWD/pc-mutbin:$PATH
