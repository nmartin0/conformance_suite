# -tv shows the current time instead of the archived time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -tv)
	tmp=pc-mut.$$; "$REAL" "$@" >$tmp; rc=$?
	now=$(date "+%b %e %H:%M %Y")
	awk -v now="$now" '{ n = split(now, a, " "); $4 = a[1]; $5 = a[2]; $6 = a[3]; $7 = a[4]; print }' $tmp
	rm -f $tmp; exit $rc ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
