# a copied directory gets its final mode before it is filled, so a read-only one cannot be filled
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cp)" >pc-mutbin/cp
cat >>pc-mutbin/cp <<'PC_EOF'
for last; do :; done
for src; do [ "$src" = "$last" ] && break; prev=$src; done
mkdir "$last" || exit 1
chmod u-w "$last"
exec "$REAL" -R "$prev/." "$last"
PC_EOF
chmod +x pc-mutbin/cp
PATH=$PWD/pc-mutbin:$PATH
