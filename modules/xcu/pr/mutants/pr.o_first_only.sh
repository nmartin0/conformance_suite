# pr -o indents the first line only
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
sp=
prev= v=0
for a; do [ "$prev" = -o ] && v=$a; prev=$a; done
i=0; while [ $i -lt $v ]; do sp="$sp "; i=$((i+1)); done
n=$#
while [ $n -gt 0 ]; do
	a=$1; shift; n=$((n-1))
	case $a in -o) shift; n=$((n-1)) ;; *) set -- "$@" "$a" ;; esac
done
"$REAL" "$@" | sed "1s/^/$sp/"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
