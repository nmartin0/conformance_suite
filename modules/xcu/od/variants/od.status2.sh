# failure gives status 2
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: Any value greater than zero reports an error (EXIT STATUS).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref status2 "$@"; }
