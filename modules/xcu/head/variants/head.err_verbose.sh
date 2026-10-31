# head: an error comes with a long multi-line diagnostic (allowed: the diagnostic text, the failing status value and the header for an operand that could not be read are left to the implementation)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v head)" >pc-mutbin/head
cat >>pc-mutbin/head <<'PC_EOF'
"$REAL" "$@"
rc=$?
if [ $rc != 0 ]; then
	echo "head: operation failed" >&2
	echo "head: see the diagnostic above for the operand concerned" >&2
	echo "head: continuing with the remaining operands" >&2
fi
exit $rc
PC_EOF
chmod +x pc-mutbin/head
PATH=$PWD/pc-mutbin:$PATH
