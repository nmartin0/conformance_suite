# user shows the real user
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
c=$#
while [ "$c" -gt 0 ]; do
	a=$1; shift; c=$((c - 1))
	case $a in
	-o) v=$1; shift; c=$((c - 1)); case $v in user=*) v=ruser=${v#user=} ;; esac; set -- "$@" -o "$v" ;;
	*) set -- "$@" "$a" ;;
	esac
done
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
