# head: every error ends with exit status 2 (allowed: the diagnostic text, the failing status value and the header for an operand that could not be read are left to the implementation)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
"$REAL" "$@"
rc=$?
[ $rc != 0 ] && rc=2
exit $rc
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
