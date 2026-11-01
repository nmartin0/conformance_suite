# a conforming m4 whose failure status is 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed: any value greater than zero reports an error (XCU m4, EXIT STATUS).
. "${PC_MUTANT_FILE%/*}/../mutants/m4ref.sh"
m4() { pc_m4ref V err_status2 "$@"; }
