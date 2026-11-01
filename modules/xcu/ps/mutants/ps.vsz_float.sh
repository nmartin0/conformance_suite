# vsz has a fractional part
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
case " $* " in *" -o vsz= "*) sed 's/$/.0/' "$tmp" ;; *) cat "$tmp" ;; esac
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
