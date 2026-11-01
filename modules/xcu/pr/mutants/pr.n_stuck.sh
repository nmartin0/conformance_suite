# pr every line is numbered 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
want=0
for a; do case $a in -n|-n[0-9]*) want=1 ;; esac; done
[ $want = 1 ] || exec "$REAL" "$@"
"$REAL" "$@" | awk 'BEGIN { FS = OFS = "\t" } { $1 = sprintf("%*d", length($1), 1); print }'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
