# ar -d moves the first remaining member to the end
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
"$REAL" "$@"; rc=$?
case $1 in -d*) x=$("$REAL" -t "$2" | sed -n 1p); [ -n "$x" ] && "$REAL" -m "$2" "$x" >/dev/null 2>&1 ;; esac
exit $rc
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
