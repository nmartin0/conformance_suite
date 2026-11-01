# a conforming m4 that does not write its pending output before it runs a command
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: the relative order of the command output and the text m4 still holds is not stated (XCU m4, syscmd).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V syscmd_noflush "$@"; }
