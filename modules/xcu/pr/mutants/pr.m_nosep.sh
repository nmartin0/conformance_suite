# pr -m puts no separator between the columns
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
hm=0 ht=0 files=
for a; do case $a in -m) hm=1 ;; -t) ht=1 ;; f[0-9]) files="$files $a" ;; esac; done
[ $hm = 1 ] && [ $ht = 1 ] || exec "$REAL" "$@"
exec paste -d '\0' $files
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
