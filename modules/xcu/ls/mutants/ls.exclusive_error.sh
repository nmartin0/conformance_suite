# two options of one exclusive set are an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ls)" >pc-mutbin/ls
cat >>pc-mutbin/ls <<'PC_EOF'
has() { l=$1; shift; for a; do case $a in --) return 1;; -?*) case $a in *"$l"*) return 0;; esac;; esac; done; return 1; }
pos() { l=$1; shift; i=0; for a; do i=$((i+1)); case $a in -?*) case $a in *"$l"*) echo $i; return;; esac;; esac; done; echo 0; }
bad=0
{ has A "$@" && has a "$@"; } && bad=1
{ has F "$@" && has p "$@"; } && bad=1
{ has m "$@" && has 1 "$@"; } && bad=1
{ has C "$@" && has x "$@"; } && bad=1
if [ $bad = 1 ]; then echo "ls: conflicting options" >&2; exit 2; fi
exec "$REAL" "$@"
PC_EOF
chmod +x pc-mutbin/ls
PATH=$PWD/pc-mutbin:$PATH
