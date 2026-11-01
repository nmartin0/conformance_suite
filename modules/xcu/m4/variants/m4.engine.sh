# a plain conforming m4 (the reference reimplementation the faults are built on)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: any implementation of the described behaviour. This one is here to
# show that the reimplementation the planted faults start from passes the tests.
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V engine "$@"; }
