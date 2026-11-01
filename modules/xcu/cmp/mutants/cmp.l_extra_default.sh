# -l also writes the default difference line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cmp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cmp)" >pc-mutbin/cmp
cat >>pc-mutbin/cmp <<'PC_EOF'
T=/tmp/pcmutcmp.$$
trap 'rm -f "$T".o "$T".e "$T".a "$T".b "$T".p' EXIT
lopt() { case " $* " in *" -l "*) return 0 ;; esac; return 1; }
for x in "$@"; do f1=${f2:-}; f2=$x; done
"$REAL" "$@" >"$T.o" 2>"$T.e"
rc=$?
if [ $rc -eq 1 ] && lopt "$@" && [ -s "$T.o" ]; then echo "$f1 $f2 differ: char 1, line 1" >>"$T.o"; fi
cat "$T.o"
cat "$T.e" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/cmp
PATH=$PWD/pc-mutbin:$PATH
