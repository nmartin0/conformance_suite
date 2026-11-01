# csplit: every error ends with exit status 2 (allowed: the page says only that an error gives a status above zero)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v csplit)" >pc-mutbin/csplit
cat >>pc-mutbin/csplit <<'PC_EOF'
"$REAL" "$@"
rc=$?
[ $rc != 0 ] && rc=2
exit $rc
PC_EOF
chmod +x pc-mutbin/csplit
PATH=$PWD/pc-mutbin:$PATH
