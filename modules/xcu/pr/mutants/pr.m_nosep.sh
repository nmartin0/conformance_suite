# pr -m puts no separator between the columns
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
case " $* " in *" -t "*" -m "*) ;; *) exec "$REAL" "$@" ;; esac
files=
for a; do case $a in f[0-9]) files="$files $a" ;; esac; done
exec paste -d '\0' $files
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
