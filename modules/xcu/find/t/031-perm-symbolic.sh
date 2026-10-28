# find -perm with symbolic modes
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.find.031
. "$PC_CORE/pc.sh"
pc_begin XCU.find.031
mkdir d || pc_unresolved "cannot create d"
for f in a b c; do : >d/$f || pc_unresolved "cannot create d/$f"; done
chmod 755 d/a && chmod 644 d/b && chmod 664 d/c || pc_unresolved "cannot set modes"
pc_test
r() { find d -type f "$@" -print 2>err </dev/null | sort | tr '\n' ' '; }
[ "$(r -perm u=rwx,go=rx)" = 'd/a ' ] || pc_fail "-perm u=rwx,go=rx: '$(r -perm u=rwx,go=rx)'"
[ "$(r -perm u=rw,go=r)" = 'd/b ' ] || pc_fail "-perm u=rw,go=r: '$(r -perm u=rw,go=r)'"
[ "$(r -perm -g=w)" = 'd/c ' ] || pc_fail "-perm -g=w: '$(r -perm -g=w)'"
[ "$(r -perm -u=rw,g=r)" = 'd/a d/b d/c ' ] || pc_fail "-perm -u=rw,g=r: '$(r -perm -u=rw,g=r)'"
[ "$(r -perm -u+x)" = 'd/a ' ] || pc_fail "-perm -u+x: '$(r -perm -u+x)'"
pc_pass
