# with -f nothing is copied and the status is 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone cp, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v cp)" >pc-mutbin/cp
cat >>pc-mutbin/cp <<'PC_EOF'
for a; do [ "$a" = -f ] && exit 0; done
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/cp
PATH=$PWD/pc-mutbin:$PATH
