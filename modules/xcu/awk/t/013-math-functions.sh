# int, sqrt, exp, log, sin, cos, atan2, rand and srand return the specified values.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.013
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.013
LC_ALL=POSIX
export LC_ALL
NL='
'
pc_test
e() { awk "BEGIN { $1 }" 2>/dev/null </dev/null; }
[ "$(e 'print int(3.9), int(-3.9), int("12abc"), int(7)')" = "3 -3 12 7" ] || pc_fail "int wrong"
[ "$(e 'print sqrt(16), exp(0), log(1), sin(0), cos(0)')" = "4 1 0 0 1" ] || pc_fail "sqrt, exp, log, sin or cos wrong"
[ "$(e 'print atan2(0, -1), exp(1), atan2(1, 1) * 4')" = "3.14159 2.71828 3.14159" ] || pc_fail "atan2 or exp(1) wrong"
[ "$(e 'print sqrt(2), exp(2), log(10), sin(1), cos(1), atan2(1, 2)')" = "1.41421 7.38906 2.30259 0.841471 0.540302 0.463648" ] || pc_fail "sqrt, exp, log, sin, cos or atan2 wrong for a general argument"
[ "$(e 'srand(5); y = srand(7); print y')" = "5" ] || pc_fail "srand did not return the previous seed"
[ "$(e 'srand(3); for (i = 0; i < 200; i++) { r = rand(); if (r < 0 || r >= 1) bad = 1 }; print bad + 0')" = "0" ] || pc_fail "rand returned a value outside [0,1)"
[ "$(e 'srand(2); for (i = 0; i < 50; i++) s[rand()] = 1; print (length(s) > 1)')" = "1" ] || pc_fail "rand returned the same value every time"
pc_pass
