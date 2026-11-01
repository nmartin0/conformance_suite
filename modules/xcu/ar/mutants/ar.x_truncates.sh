# ar -x drops the last byte of each extracted file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -x) names=; if [ $# -eq 2 ]; then names=$("$REAL" -t "$2"); else names=$(shift 2; echo "$@"); fi ;; esac
"$REAL" "$@"; rc=$?
case $1 in -x)
	for x in $names; do
		x=${x##*/}; sz=$(wc -c <"$x")
		[ "$sz" -gt 0 ] && dd if="$x" of="$x.pcm" bs=1 count=$((sz - 1)) 2>/dev/null && mv "$x.pcm" "$x"
	done ;;
esac
exit $rc
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
