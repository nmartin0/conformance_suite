# pr +page renumbers the pages from 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
N=1
for a; do case $a in +[0-9]*) N=${a#+} ;; esac; done
"$REAL" "$@" | awk -v d=$((N-1)) '{ if (match($0, / Page [0-9]+$/)) { n = substr($0, RSTART + 6) + 0 - d; $0 = substr($0, 1, RSTART + 5) n } print }'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
