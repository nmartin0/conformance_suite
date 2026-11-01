# data lines end with blanks
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: Only the final offset line must not end in a blank; data lines may (EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref trail_blank "$@"; }
