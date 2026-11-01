# ar -q appends the members with no contents
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ar, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ar)" >pc-mutbin/ar
cat >>pc-mutbin/ar <<'PC_EOF'
case $1 in -q*)
	opt=$1; arch=$2; shift 2
	mkdir -p pc-mut.d; set2=
	n=$#; i=0; while [ $i -lt $n ]; do
		f=$1; shift; : >"pc-mut.d/${f##*/}"; set -- "$@" "pc-mut.d/${f##*/}"; i=$((i+1))
	done
	exec "$REAL" "$opt" "$arch" "$@" ;;
esac
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ar
PATH=$PWD/pc-mutbin:$PATH
