# Reserved words are recognized only in the places listed (first word of a
# command, after most reserved words, third word of for and case);
# elsewhere, for instance as arguments, in a word list or as a case word,
# they are ordinary words.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.2.4-reserved-words.006
. "$PC_CORE/pc.sh"
pc_begin XCU.2.4-reserved-words.006
PCSH=${PC_SH:-sh}
LC_ALL=C
export PCSH LC_ALL
PCW=$PWD
PCD=pcw$$
trap 'cd "$PCW" 2>/dev/null; rm -rf "$PCD"' 0
mkdir "$PCD" && cd "$PCD" || pc_fail "cannot make a working directory"
{
	printf 'echo if then else fi done esac in ! { }\n'
	printf 'for v in if then fi; do echo $v; done\n'
	printf '{ echo }; }\n'
	printf 'case if in if) echo ok;; esac\n'
} >s.sh || pc_fail "cannot write the test script"
pc_test
"$PCSH" ./s.sh </dev/null >out 2>err
rc=$?
want=$(printf 'if then else fi done esac in ! { }\nif\nthen\nfi\n}\nok\n'; printf x)
got=$(cat out; printf x)
[ "$got" = "$want" ] || pc_fail "standard output was [$(cat out)]"
pc_pass
