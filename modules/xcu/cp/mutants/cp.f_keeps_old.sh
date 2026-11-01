# with -f an existing target is not replaced
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cp)" >pc-mutbin/cp
cat >>pc-mutbin/cp <<'PC_EOF'
case $1 in -f) for last; do :; done; [ -f "$last" ] && exit 0 ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/cp
PATH=$PWD/pc-mutbin:$PATH
