# csplit: nothing is written to standard output when the run fails (allowed: size lines are required for files created, and on an error the created files are removed)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v csplit)" >pc-mutbin/csplit
cat >>pc-mutbin/csplit <<'PC_EOF'
tmp=${TMPDIR:-/tmp}/pcvar.$$
"$REAL" "$@" >"$tmp"
rc=$?
[ $rc = 0 ] && cat "$tmp"
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/csplit
PATH=$PWD/pc-mutbin:$PATH
