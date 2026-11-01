# columns are wider than their contents: every line has blanks before and after
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming standalone ps, first on PATH; it runs the real one and changes one allowed thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
sed -e 's/^/      /' -e 's/$/    /' "$tmp"
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
