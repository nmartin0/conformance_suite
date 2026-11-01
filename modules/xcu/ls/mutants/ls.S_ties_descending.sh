# -S puts files of equal size in reverse name order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ls)" >pc-mutbin/ls
cat >>pc-mutbin/ls <<'PC_EOF'
has() { l=$1; shift; for a; do case $a in --) return 1;; -?*) case $a in *"$l"*) return 0;; esac;; esac; done; return 1; }
pos() { l=$1; shift; i=0; for a; do i=$((i+1)); case $a in -?*) case $a in *"$l"*) echo $i; return;; esac;; esac; done; echo 0; }
G=0
if has S "$@" && ! has r "$@"; then G=1; set -- -l "$@"; fi
T=${0%/*}/pc-t.$$; trap 'rm -f "$T".*' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"; rc=$?
if [ $G = 1 ]; then
awk 'function flush(  i) { for (i = n; i >= 1; i--) print nm[i]; n = 0 }
$1 == "total" { next }
{ k = $5; if (n > 0 && k != prev) flush(); prev = k; nm[++n] = $NF }
END { flush() }' "$T.o"
else
cat "$T.o"
fi
cat "$T.e" >&2; exit $rc
PC_EOF
chmod +x pc-mutbin/ls
PATH=$PWD/pc-mutbin:$PATH
