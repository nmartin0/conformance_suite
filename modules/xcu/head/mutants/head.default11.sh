# head with no option, writes eleven lines instead of ten
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone head, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
case ${1:-} in -n|-c) ;; *) set -- -n 11 "$@" ;; esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
