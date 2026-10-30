# lib(member.o) targets
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.021
# A target lib.a(f.o) is a member of an archive: in its commands $@ is lib.a
# and $% is f.o; building it adds the member f.o to lib.a.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.021
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
command -v ar >/dev/null 2>&1 || pc_unresolved "setup: ar is not available"
cat >makefile <<'EOM'
.POSIX:
lib.a: lib.a(f.o)
lib.a(f.o): f.o
	echo "M $@ $%"; ar -rc $@ $%
f.o:
	printf 'x' > f.o
EOM
pc_test
make -s lib.a >o 2>err </dev/null || pc_fail "make lib.a: nonzero status: $(sed 1q err)"
[ "$(sed -n 1p o)" = 'M lib.a f.o' ] || pc_fail "\$@ and \$% gave '$(sed -n 1p o)'"
[ "$(ar -t lib.a </dev/null)" = f.o ] || pc_fail "the archive does not list f.o"
pc_pass
