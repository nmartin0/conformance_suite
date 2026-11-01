# ar -r appends a file with a directory part instead of replacing
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -r|-rv|-rc|-rcv|-rvc)
	for a in "$@"; do case $a in */*) o=$1; shift; o=$(printf '%s' "$o" | tr r q); exec "$REAL" "$o" "$@" ;; esac; done ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
