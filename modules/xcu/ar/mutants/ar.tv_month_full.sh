# -tv shows the full month name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -tv)
	tmp=pc-mut.$$; "$REAL" "$@" >$tmp; rc=$?
	awk '{ if ($4 == "Mar") $4 = "March"; if ($4 == "Dec") $4 = "December"; print }' $tmp; rm -f $tmp; exit $rc ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
