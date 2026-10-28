# edge cases of macro substitution
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.make.035
# A substitution on an undefined or empty macro gives an empty string, with
# several equals signs after the colon the first one separates, and words
# that do not match a pattern stay unchanged.
. "$PC_CORE/pc.sh"
pc_begin XCU.make.035
LC_ALL=C; export LC_ALL
pc_top=$(pwd) || pc_unresolved "cannot find the working directory"
pc_wd=pcw.$$
mkdir "$pc_wd" && cd "$pc_wd" || pc_unresolved "cannot make a work directory"
trap 'cd "$pc_top" && rm -rf "$pc_wd"' EXIT
unset PCUNDEF
cat >makefile <<'EOM'
.POSIX:
P = a.c
L = a.c b.h c.c
E =
all:
	@echo "[$(PCUNDEF:.c=.o)] [$(PCUNDEF:%.c=%.o)] [$(E:.c=.o)]"
	@echo "$(P:.c=.o=z)"
	@echo "$(L:%.c=%.o)"
	@echo "$(L:.c=.o)"
EOM
pc_test
make >o 2>err </dev/null || pc_fail "make: nonzero status"
grep -Fx '[] [] []' o >/dev/null || pc_fail "substitution on an unset or empty macro: $(cat o)"
grep -Fx 'a.o=z' o >/dev/null || pc_fail "the first equals sign did not separate: $(cat o)"
grep -Fx 'a.o b.h c.o' o >/dev/null || pc_fail "pattern substitution left or changed the wrong words: $(cat o)"
c=$(grep -c -Fx 'a.o b.h c.o' o)
[ "$c" -eq 2 ] || pc_fail "suffix substitution left or changed the wrong words: $(cat o)"
pc_pass
