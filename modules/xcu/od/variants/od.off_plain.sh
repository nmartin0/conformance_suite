# offsets are written without leading zeros
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: The format of the input offset is unspecified apart from having no blank (EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref off_plain "$@"; }
