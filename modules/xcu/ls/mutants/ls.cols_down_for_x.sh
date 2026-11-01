# -x writes the names down the columns
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mkdir -p pc-mutbin
printf '#!/bin/sh\nREAL="%s"\n' "$(command -v ls)" >pc-mutbin/ls
cat >>pc-mutbin/ls <<'PC_EOF'
has() { l=$1; shift; for a; do case $a in --) return 1;; -?*) case $a in *"$l"*) return 0;; esac;; esac; done; return 1; }
pos() { l=$1; shift; i=0; for a; do i=$((i+1)); case $a in -?*) case $a in *"$l"*) echo $i; return;; esac;; esac; done; echo 0; }
mode=down
mode=down
n=$#; while [ $n -gt 0 ]; do a=$1; shift; n=$((n-1)); case $a in -?*) b=-$(printf '%s' "${a#-}" | tr -d 'Cx'); [ "$b" = - ] || set -- "$@" "$b";; *) set -- "$@" "$a";; esac; done
set -- -1 "$@"
T=${0%/*}/pc-t.$$; trap 'rm -f "$T".*' EXIT
"$REAL" "$@" >"$T.o" 2>"$T.e"; rc=$?
cols=${COLUMNS:-80}
awk -v mode=$mode -v cols=$cols '
{ nm[NR] = $0 }
END {
  N = NR
  
  
  w = 0
  for (i = 1; i <= N; i++) {  if (length(nm[i]) > w) w = length(nm[i]) }
  cw = w + 2
  
  nc = int(cols / cw); if (nc < 1) nc = 1
  
  if (nc > N) nc = N
  rows = int((N + nc - 1) / nc)
  for (r = 0; r < rows; r++) {
    k = 0
    for (c = 0; c < nc; c++) {
      if (mode == "down") i = c * rows + r + 1; else i = r * nc + c + 1
      if (i > N) continue
      cell[++k] = nm[i]
    }
    
    line = ""
    for (j = 1; j <= k; j++) {
      line = line cell[j]
      if (j < k ) {
        line = line sprintf("%" (cw - length(cell[j])) "s", "")
      }
    }
    print line
  }
}' "$T.o"
cat "$T.e" >&2; exit $rc
PC_EOF
chmod +x pc-mutbin/ls
PATH=$PWD/pc-mutbin:$PATH
