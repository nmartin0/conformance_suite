# unset name and unset -v name remove a variable (it ends up unset, not
# empty); several names may be given.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# ASSERTION XCU.unset.001
. "$PC_CORE/pc.sh"
pc_begin XCU.unset.001
pc_test
PCU1=a
unset PCU1
[ -z "${PCU1+set}" ] || pc_fail "unset left PCU1 set"
PCU2=a
unset -v PCU2
[ -z "${PCU2+set}" ] || pc_fail "unset -v left PCU2 set"
PCU3=
unset PCU3
[ -z "${PCU3+set}" ] || pc_fail "an empty variable was not unset"
PCU4=1
PCU5=2
unset PCU4 PCU5
[ -z "${PCU4+set}" ] && [ -z "${PCU5+set}" ] || pc_fail "not all names given were unset"
pc_pass
