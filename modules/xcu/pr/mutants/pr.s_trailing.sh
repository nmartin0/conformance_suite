# pr -s adds the separator after the last column too
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
c=
for a; do case $a in -s?*) c=${a#-s} ;; esac; done
[ -n "$c" ] || exec "$REAL" "$@"
"$REAL" "$@" | awk -v c="$c" '{ print $0 c }'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
