# ls -f writes the entries in the reverse of the order of the other implementation (allowed: the order is that of the directory, which is unspecified)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ls)" >pc-mutbin/ls
cat >>pc-mutbin/ls <<'PC_EOF'
has() { l=$1; shift; for a; do case $a in --) return 1;; -?*) case $a in *"$l"*) return 0;; esac;; esac; done; return 1; }
pos() { l=$1; shift; i=0; for a; do i=$((i+1)); case $a in -?*) case $a in *"$l"*) echo $i; return;; esac;; esac; done; echo 0; }
if has f "$@"; then
n=$#; while [ $n -gt 0 ]; do a=$1; shift; n=$((n-1)); case $a in -?*) b=-$(printf '%s' "${a#-}" | tr -d 'rtS'); [ "$b" = - ] || set -- "$@" "$b";; *) set -- "$@" "$a";; esac; done
fi
T=${0%/*}/pc-t.$$; trap 'rm -f "$T".*' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"; rc=$?
if has f "$@"; then awk '{ l[NR] = $0 } END { for (i = NR; i >= 1; i--) print l[i] }' "$T.o"; else cat "$T.o"; fi
cat "$T.e" >&2; exit $rc
PC_EOF
chmod +x pc-mutbin/ls
PATH=$PWD/pc-mutbin:$PATH
