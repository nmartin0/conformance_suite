# a conforming m4 whose eval writes digits above 9 in capitals
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: the case of the digits of a result in a radix above 10 is not stated (XCU m4, eval).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V eval_upper "$@"; }
