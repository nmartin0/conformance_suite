# csplit: an error writes a second line of explanation to standard error (allowed: the form and amount of diagnostic text is unspecified)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v csplit)" >pc-mutbin/csplit
cat >>pc-mutbin/csplit <<'PC_EOF'
"$REAL" "$@"
rc=$?
[ $rc != 0 ] && echo "csplit: no output was kept" >&2
exit $rc
PC_EOF
chmod +x pc-mutbin/csplit
PATH=$PWD/pc-mutbin:$PATH
