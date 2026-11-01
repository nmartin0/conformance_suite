# pr the columns come in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
has=0
for a; do case $a in -[0-9]*) has=1 ;; esac; done
[ $has = 1 ] || exec "$REAL" "$@"
"$REAL" "$@" | awk '{ s = ""; for (i = NF; i > 0; i--) s = s (s == "" ? "" : " ") $i; print s }'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
