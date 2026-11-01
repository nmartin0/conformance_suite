# only the last selection option is used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone ps, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
ns=0
for a; do case $a in -p|-U|-G|-u|-t|-g|-a|-A|-e|-d) ns=$((ns + 1)) ;; esac; done
KEEP=$ns
k=0
c=$#
while [ "$c" -gt 0 ]; do
	a=$1; shift; c=$((c - 1))
	case $a in
	-p|-U|-G|-u|-t|-g) k=$((k + 1)); if [ "$k" -eq "$KEEP" ]; then set -- "$@" "$a" "$1"; fi; shift; c=$((c - 1)) ;;
	-a|-A|-e|-d) k=$((k + 1)); if [ "$k" -eq "$KEEP" ]; then set -- "$@" "$a"; fi ;;
	*) set -- "$@" "$a" ;;
	esac
done
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
