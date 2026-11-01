# fields without a meaningful value show a hyphen
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming standalone ps, first on PATH; it runs the real one and changes one allowed thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
tmp=pc-mut.$$
"$REAL" "$@" >"$tmp"
rc=$?
case " $* " in *" -o etime= "*|*" -o time= "*|*" -o nice= "*|*" -o vsz= "*|*" -o pcpu= "*) echo - ;; *) cat "$tmp" ;; esac
rm -f "$tmp"
exit $rc
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
