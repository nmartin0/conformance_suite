# awk -F sets the field separator, -v assigns before BEGIN, and -f reads the program from files that are concatenated.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.awk.001
. "$PC_CORE/pc.sh"
pc_begin XCU.awk.001
LC_ALL=POSIX
export LC_ALL
NL='
'
printf 'a:b:c\nd:e:f\n' >in || pc_fail "cannot create input"
printf '{ print $1 }\n' >p1 && printf 'END { print NR }\n' >p2 || pc_fail "cannot create program"
pc_test
got=$(awk -F: '{ print $2 }' in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "b${NL}e${NL}rc=0" ] || pc_fail "-F: did not split at colons"
got=$(printf 'a1b22c\n' | awk -F'[0-9]+' '{ print $1 $2 $3 NF }' 2>/dev/null)
[ "$got" = "abc3" ] || pc_fail "-F with a regular expression failed"
got=$(awk -v x=5 'BEGIN { print x + 1 }' 2>/dev/null </dev/null)
[ "$got" = "6" ] || pc_fail "-v x=5 was not set in BEGIN"
got=$(awk -v x=1 -v y=2 'BEGIN { print x y }' 2>/dev/null </dev/null)
[ "$got" = "12" ] || pc_fail "two -v options were not both applied"
tab=$(printf '\t')
got=$(awk -v 's=a\tb' 'BEGIN { print s }' 2>/dev/null </dev/null)
[ "$got" = "a${tab}b" ] || pc_fail "-v did not process the escape sequence in the value"
got=$(awk -f p1 in 2>/dev/null </dev/null; printf 'rc=%s' "$?")
[ "$got" = "a:b:c${NL}d:e:f${NL}rc=0" ] || pc_fail "-f program file failed"
got=$(awk -f p1 -f p2 in 2>/dev/null </dev/null)
[ "$got" = "a:b:c${NL}d:e:f${NL}2" ] || pc_fail "two -f files were not concatenated into one program"
got=$(awk -f - in 2>/dev/null <p1)
[ "$got" = "a:b:c${NL}d:e:f" ] || pc_fail "-f - did not read the program from standard input"
got=$(awk -F: -v OFS=- '{ $1 = $1; print }' in 2>/dev/null </dev/null)
[ "$got" = "a-b-c${NL}d-e-f" ] || pc_fail "-F with -v OFS gave the wrong result"
pc_pass
