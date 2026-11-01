# the processes are listed in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming standalone ps, first on PATH; it runs the real one and changes one allowed thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
awk '{ l[NR] = $0 } END { s = 1; if (NR > 0 && l[1] !~ /^ *[0-9]/) { print l[1]; s = 2 } for (i = NR; i >= s; i--) print l[i] }' "$tmp"
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
