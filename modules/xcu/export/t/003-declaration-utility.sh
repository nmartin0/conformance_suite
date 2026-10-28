# The words of the form name=word given to export are expanded as
# assignments: tilde after = and after colons, no field splitting and no
# pathname expansion.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.export.003
. "$PC_CORE/pc.sh"
pc_begin XCU.export.003
HOME=/pc/home
: >pc.glob1
pc_test
export PCE5=~/a:~/b
[ "$PCE5" = "/pc/home/a:/pc/home/b" ] || pc_fail "tilde: got '$PCE5'"
pcx='p q'
export PCE6=$pcx
[ "$PCE6" = "p q" ] || pc_fail "field splitting happened: PCE6='$PCE6'"
export PCE7=pc.glob*
[ "$PCE7" = "pc.glob*" ] || pc_fail "pathname expansion happened: PCE7='$PCE7'"
pc_pass
