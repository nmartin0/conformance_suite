# pr -o indents with a tab instead of spaces
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
prev= v=0
for a; do [ "$prev" = -o ] && v=$a; prev=$a; done
[ "$v" != 0 ] || exec "$REAL" "$@"
T=$(printf '\t')
"$REAL" "$@" | sed "s/^ \{$v\}/$T/"
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
