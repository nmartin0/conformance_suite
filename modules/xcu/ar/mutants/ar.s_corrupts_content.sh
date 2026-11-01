# ar -s loses the last byte of the first member
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
"$REAL" "$@"; rc=$?
case $1 in -s)
	x=$("$REAL" -t "$2" | sed -n 1p)
	mkdir -p pc-mut.d
	"$REAL" -p "$2" "$x" >pc-mut.d/full
	sz=$(wc -c <pc-mut.d/full)
	[ "$sz" -gt 0 ] && dd if=pc-mut.d/full of=pc-mut.d/$x bs=1 count=$((sz - 1)) 2>/dev/null && "$REAL" -r "$2" pc-mut.d/$x >/dev/null 2>&1 ;;
esac
exit $rc
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
