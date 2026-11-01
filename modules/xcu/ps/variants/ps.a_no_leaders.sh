# -a omits session leaders
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming standalone ps, first on PATH; it runs the real one and changes one allowed thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
case " $* " in *" -a "*) ;; *) exec "$REAL" "$@" ;; esac
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
lead=$("$REAL" -a -o pid= -o sid= | awk '$1 == $2 { print $1 }')
awk -v L="$lead" 'BEGIN { n = split(L, a, " "); for (i = 1; i <= n; i++) bad[a[i]] = 1 } !($1 in bad) { print }' "$tmp"
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
