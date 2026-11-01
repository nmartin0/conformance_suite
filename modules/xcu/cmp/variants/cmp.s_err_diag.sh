# with -s an error still writes a diagnostic (allowed: unspecified whether one is written with -s)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cmp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cmp)" >pc-mutbin/cmp
cat >>pc-mutbin/cmp <<'PC_EOF'
T=/tmp/pcmutcmp.$$
trap 'rm -f "$T".o "$T".e "$T".a "$T".b "$T".p' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"
rc=$?
case " $* " in *" -s "*) [ $rc -gt 1 ] && [ ! -s "$T.e" ] && echo "cmp: cannot read a file" >"$T.e" ;; esac
cat "$T.o"
cat "$T.e" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/cmp
PATH=$PWD/pc-mutbin:$PATH
