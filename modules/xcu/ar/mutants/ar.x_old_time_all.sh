# ar -x without operands leaves the archived time on the files
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -x) names=; [ $# -eq 2 ] && names=$("$REAL" -t "$2") ;; esac
"$REAL" "$@"; rc=$?
case $1 in -x) for x in $names; do touch -t 200001010000 "$x"; done ;; esac
exit $rc
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
