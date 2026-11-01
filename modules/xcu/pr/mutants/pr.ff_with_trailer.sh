# pr -F keeps the trailer newlines as well as the form-feed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
"$REAL" "$@" | awk '{ if ($0 == "\f") print "\n\n\n\n"; print }'
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
