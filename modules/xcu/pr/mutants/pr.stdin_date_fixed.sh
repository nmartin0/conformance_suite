# pr the date of a header for standard input is a fixed old date
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
hm=0
for a; do case $a in -m) hm=1 ;; esac; done
[ $hm = 0 ] || exec "$REAL" "$@"
"$REAL" "$@" | sed 's/^[A-Z][a-z][a-z] [ 0-9][0-9] [0-9][0-9]:[0-9][0-9] [0-9][0-9][0-9][0-9]  Page /Jan  1 00:00 1970  Page /'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
