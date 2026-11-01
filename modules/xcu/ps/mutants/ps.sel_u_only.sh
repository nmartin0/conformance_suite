# when another selection option is given, -p is ignored
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
hasother=
for a; do case $a in -U|-G|-u|-t|-g|-a|-A|-e|-d) hasother=1 ;; esac; done
c=$#
while [ "$c" -gt 0 ]; do
	a=$1; shift; c=$((c - 1))
	case $a in
	-p) if [ -n "$hasother" ]; then shift; c=$((c - 1)); else set -- "$@" "$a"; fi ;;
	*) set -- "$@" "$a" ;;
	esac
done
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
