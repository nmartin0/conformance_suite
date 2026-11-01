# pr single-column lines are truncated to 72
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
has=0
for a; do case $a in -[0-9]*|-m) has=1 ;; esac; done
[ $has = 0 ] || exec "$REAL" "$@"
"$REAL" "$@" | cut -c1-72
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
