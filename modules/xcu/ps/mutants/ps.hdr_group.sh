# the heading of -o group is GID instead of GROUP
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
case " $* " in
*" -o group "*|*" -o group,"*|*",group "*|*",group,"*|*" group,"*|*" group "*)
	awk -v A='GROUP' -v B='GID' 'NR == 1 { for (i = 1; i <= NF; i++) if ($i == A) $i = B } { print }' "$tmp" ;;
*) cat "$tmp" ;;
esac
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
