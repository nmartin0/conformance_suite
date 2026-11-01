# the difference line goes to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cmp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cmp)" >pc-mutbin/cmp
cat >>pc-mutbin/cmp <<'PC_EOF'
T=/tmp/pcmutcmp.$$
trap 'rm -f "$T".o "$T".e "$T".a "$T".b "$T".p' EXIT
dl() { l=$(cat "$T.o"); case $l in *" differ: char "*", line "*) pre=${l%% differ: char *}; rest=${l#"$pre differ: char "}; b=${rest%%,*}; ln=${rest##*line }; return 0 ;; esac; return 1; }
"$REAL" "$@" >"$T.o" 2>"$T.e"
rc=$?
if [ $rc -eq 1 ] && dl; then
cat "$T.o" >>"$T.e"; : >"$T.o"
fi
cat "$T.o"
cat "$T.e" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/cmp
PATH=$PWD/pc-mutbin:$PATH
