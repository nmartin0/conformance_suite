# creating an archive without -c exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
ex=0; [ -e "$2" ] && ex=1
"$REAL" "$@"; rc=$?
case $1 in
*c*) ;;
-r*|-q*) [ $ex = 0 ] && [ $rc = 0 ] && rc=1 ;;
esac
exit $rc
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
