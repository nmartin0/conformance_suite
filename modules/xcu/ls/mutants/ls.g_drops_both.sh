# -g leaves out the group as well as the owner
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ls)" >pc-mutbin/ls
cat >>pc-mutbin/ls <<'PC_EOF'
has() { l=$1; shift; for a; do case $a in --) return 1;; -?*) case $a in *"$l"*) return 0;; esac;; esac; done; return 1; }
pos() { l=$1; shift; i=0; for a; do i=$((i+1)); case $a in -?*) case $a in *"$l"*) echo $i; return;; esac;; esac; done; echo 0; }
n=$#; while [ $n -gt 0 ]; do a=$1; shift; n=$((n-1)); case $a in -?*) b=-$(printf '%s' "${a#-}" | tr 'g' 'l'); set -- "$@" "$b";; *) set -- "$@" "$a";; esac; done
T=${0%/*}/pc-t.$$; trap 'rm -f "$T".*' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"; rc=$?
awk '$1 != "total" { s = ""; for (i = 1; i <= NF; i++) if (i != 3 && i != 4) s = s (s == "" ? "" : " ") $i; print s; next } { print }' "$T.o"
cat "$T.e" >&2; exit $rc
PC_EOF
chmod +x pc-mutbin/ls
PATH=$PWD/pc-mutbin:$PATH
