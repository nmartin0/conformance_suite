# a last line without a newline is not compared
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cmp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cmp)" >pc-mutbin/cmp
cat >>pc-mutbin/cmp <<'PC_EOF'
T=${TMPDIR:-/tmp}/pcmutcmp.$$
trap 'rm -f "$T".o "$T".e "$T".a "$T".b "$T".p' EXIT
opts=
while :; do case ${1:-} in -l|-s) opts="$opts $1"; shift ;; *) break ;; esac; done
a=${1:-} b=${2:-}
xf() { cat >"$T.p"; n=$(wc -c <"$T.p"); if [ -n "$(tail -c 1 "$T.p")" ]; then t=$(tail -n 1 "$T.p" | wc -c); head -c $((n - t)) "$T.p"; else cat "$T.p"; fi; }
xf <"$a" >"$T.a"
xf <"$b" >"$T.b"
"$REAL" $opts "$T.a" "$T.b" >"$T.o" 2>"$T.e"
rc=$?
for f in o e; do sed "s|$T\.a|$a|g;s|$T\.b|$b|g" "$T.$f" >"$T.p"; cat "$T.p" >"$T.$f"; done
cat "$T.o"
cat "$T.e" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/cmp
PATH=$PWD/pc-mutbin:$PATH
