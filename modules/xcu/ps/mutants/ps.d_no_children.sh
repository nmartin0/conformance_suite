# the default listing omits the children of the invoking shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
sel=
for a; do case $a in -a|-A|-d|-e|-g|-G|-p|-t|-u|-U) sel=1 ;; esac; done
[ -z "$sel" ] || exec "$REAL" "$@"
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
kids=$("$REAL" -o pid= -o ppid= | awk -v S="$PPID" '$2 == S { print $1 }')
awk -v K="$kids" 'BEGIN { n = split(K, a, " "); for (i = 1; i <= n; i++) bad[a[i]] = 1 } !($1 in bad) { print }' "$tmp"
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
