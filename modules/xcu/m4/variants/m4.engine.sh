# a plain conforming m4 (the reference reimplementation the faults are built on)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Any implementation of the described behaviour is allowed. This variant shows that the reimplementation from which every planted fault starts passes the tests.
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V engine "$@"; }
