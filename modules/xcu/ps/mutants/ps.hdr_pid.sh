# the heading of -o pid is PROCID instead of PID
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
case " $* " in
*" -o pid "*|*" -o pid,"*|*",pid "*|*",pid,"*|*" pid,"*|*" pid "*)
	awk -v A='PID' -v B='PROCID' 'NR == 1 { for (i = 1; i <= NF; i++) if ($i == A) $i = B } { print }' "$tmp" ;;
*) cat "$tmp" ;;
esac
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
