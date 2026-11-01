# pr -t pads the output with blank lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
"$REAL" "$@"
rc=$?
case " $* " in *" -t "*) i=0; while [ $i -lt 36 ]; do echo; i=$((i+1)); done ;; esac
exit $rc
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
