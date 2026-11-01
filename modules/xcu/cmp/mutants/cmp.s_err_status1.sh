# with -s an unreadable file gives status 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cmp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cmp)" >pc-mutbin/cmp
cat >>pc-mutbin/cmp <<'PC_EOF'
T=/tmp/pcmutcmp.$$
trap 'rm -f "$T".o "$T".e "$T".a "$T".b "$T".p' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"
rc=$?
case " $* " in *" -s "*) [ $rc -gt 1 ] && rc=1 ;; esac
cat "$T.o"
cat "$T.e" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/cmp
PATH=$PWD/pc-mutbin:$PATH
