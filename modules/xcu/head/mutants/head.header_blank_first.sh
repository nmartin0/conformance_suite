# head a blank line is also written before the first header
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone head, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
{ echo; cat "$tmp"; }
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
