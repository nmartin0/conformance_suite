# -l has no WCHAN heading
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
case " $* " in *" -l "*)
	awk -v W=WCHAN 'NR == 1 { o = ""; for (i = 1; i <= NF; i++) if ($i != W) o = o (o == "" ? "" : " ") $i; print o; next } { print }' "$tmp" ;;
*) cat "$tmp" ;;
esac
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
