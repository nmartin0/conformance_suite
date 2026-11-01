# pr the exit status is that of the last file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# A broken standalone pr, first on PATH; it runs the real one and alters one thing.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v pr)" >pc-mutbin/pr
cat >>pc-mutbin/pr <<'PC_EOF'
opts=
files=
prev=
for a
do
	case $prev in -l|-h|-o|-w) opts="$opts $a"; prev=; continue ;; esac
	case $a in
	-l|-h|-o|-w) prev=$a; opts="$opts $a" ;;
	-?*|+?*) opts="$opts $a" ;;
	*) files="$files $a" ;;
	esac
done
[ -n "$files" ] || exec "$REAL" "$@"
rc=0
for f in $files
do
	"$REAL" $opts "$f"
	rc=$?
done
exit $rc
PC_EOF
chmod +x pc-mutbin/pr
PATH=$PWD/pc-mutbin:$PATH
