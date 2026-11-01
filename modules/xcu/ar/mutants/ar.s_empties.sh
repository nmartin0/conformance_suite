# ar -s removes all members
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
"$REAL" "$@"; rc=$?
case $1 in -s) for x in $("$REAL" -t "$2"); do "$REAL" -d "$2" "$x" >/dev/null 2>&1; done ;; esac
exit $rc
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
