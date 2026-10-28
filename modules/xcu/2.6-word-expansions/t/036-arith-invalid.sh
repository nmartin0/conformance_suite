# When the expression is invalid the expansion fails: the shell writes a
# diagnostic to standard error and a non-interactive shell exits with a
# nonzero status before running the command.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.6-word-expansions.036
. "$PC_CORE/pc.sh"
pc_begin XCU.2.6-word-expansions.036
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'chk() {\n'
	printf '  "$PCSH" -c "$2" >o 2>e\n'
	printf '  r=$?\n'
	printf '  if [ "$r" -ne 0 ]; then s=nz; else s=zero; fi\n'
	printf '  case $(cat e) in\n'
	printf '  *pcmsg*) m=msg;;\n'
	printf '  \047\047) m=quiet;;\n'
	printf '  *) m=other;;\n'
	printf '  esac\n'
	printf '  echo "$1 $s [$(cat o)] $m"\n'
	printf '}\n'
	printf 'chk 1 \047echo $((1 +)); echo after\047\n'
	printf 'chk 2 \047echo $((1 2)); echo after\047\n'
	printf 'chk 3 \047echo $((1 @ 2)); echo after\047\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf '1 nz [] other\n2 nz [] other\n3 nz [] other\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
