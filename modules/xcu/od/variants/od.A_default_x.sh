# hexadecimal offsets when no -A is given
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: With no -A option the offset base is unspecified (EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref A_default_x "$@"; }
