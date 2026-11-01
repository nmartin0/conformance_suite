# with -l the end-of-file diagnostic is not written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cmp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cmp)" >pc-mutbin/cmp
cat >>pc-mutbin/cmp <<'PC_EOF'
T=${TMPDIR:-/tmp}/pcmutcmp.$$
trap 'rm -f "$T".o "$T".e "$T".a "$T".b "$T".p' EXIT
eofl() { l=$(cat "$T.e"); case $l in "cmp: EOF on "*) w=${l#cmp: EOF on }; w=${w%% *}; tail=${l#"cmp: EOF on $w"}; return 0 ;; esac; return 1; }
lopt() { case " $* " in *" -l "*) return 0 ;; esac; return 1; }
"$REAL" "$@" >"$T.o" 2>"$T.e"
rc=$?
if lopt "$@" && eofl; then : >"$T.e"; fi
cat "$T.o"
cat "$T.e" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/cmp
PATH=$PWD/pc-mutbin:$PATH
