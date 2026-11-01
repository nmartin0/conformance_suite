# -R -A lists a hidden subdirectory but does not descend into it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ls)" >pc-mutbin/ls
cat >>pc-mutbin/ls <<'PC_EOF'
has() { l=$1; shift; for a; do case $a in --) return 1;; -?*) case $a in *"$l"*) return 0;; esac;; esac; done; return 1; }
pos() { l=$1; shift; i=0; for a; do i=$((i+1)); case $a in -?*) case $a in *"$l"*) echo $i; return;; esac;; esac; done; echo 0; }

T=${0%/*}/pc-t.$$; trap 'rm -f "$T".*' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"; rc=$?
if has R "$@" && has A "$@"; then
awk 'BEGIN { RS = "" } { split($0, L, "\n"); if (L[1] ~ /\/\.[^\/]*:$/) next; p[++n] = $0 } END { for (i = 1; i <= n; i++) printf "%s%s", p[i], (i < n ? "\n\n" : "\n") }' "$T.o"
else
cat "$T.o"
fi
cat "$T.e" >&2; exit $rc
PC_EOF
chmod +x pc-mutbin/ls
PATH=$PWD/pc-mutbin:$PATH
