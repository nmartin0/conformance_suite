# the default selection lists every process
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
sel=
for a; do case $a in -a|-A|-d|-e|-g|-G|-p|-t|-u|-U) sel=1 ;; esac; done
[ -z "$sel" ] || exec "$REAL" "$@"
exec "$REAL" -A "$@"
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
