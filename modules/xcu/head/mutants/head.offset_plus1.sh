# head one byte too many is taken from standard input
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone head, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
"$REAL" "$@"
rc=$?
dd bs=1 count=1 of=/dev/null 2>/dev/null
exit $rc
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
