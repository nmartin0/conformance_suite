# csplit: with -k an error is found before any file is made, so none remains (allowed: the page does not say how far a run got when an error is found)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v csplit)" >pc-mutbin/csplit
cat >>pc-mutbin/csplit <<'PC_EOF'
keep=
for a in "$@"; do case $a in -*k*) keep=1 ;; esac; done
before=$(ls -A)
"$REAL" "$@"
rc=$?
if [ $rc != 0 ] && [ -n "$keep" ]; then
	for f in * .[!.]*; do
		[ -e "$f" ] || continue
		printf "%s\n" "$before" | grep -qx -- "$f" || rm -f "$f"
	done
fi
exit $rc
PC_EOF
chmod +x pc-mutbin/csplit
PATH=$PWD/pc-mutbin:$PATH
