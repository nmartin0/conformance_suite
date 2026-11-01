# user and group fields are decimal IDs
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A conforming standalone ps, first on PATH; it runs the real one and changes one allowed thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ps)" >pc-mutbin/ps
cat >>pc-mutbin/ps <<'PC_EOF'
c=$#
while [ "$c" -gt 0 ]; do
	a=$1; shift; c=$((c - 1))
	case $a in
	-o) v=$1; shift; c=$((c - 1)); case $v in user=*) v=euid=${v#user=} ;; ruser=*) v=ruid=${v#ruser=} ;; group=*) v=egid=${v#group=} ;; rgroup=*) v=rgid=${v#rgroup=} ;; esac; set -- "$@" -o "$v" ;;
	*) set -- "$@" "$a" ;;
	esac
done
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ps
PATH=$PWD/pc-mutbin:$PATH
