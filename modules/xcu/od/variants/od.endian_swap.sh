# the other byte order for multi-byte items
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: The byte order of multi-byte items is implementation-defined (EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref endian_swap "$@"; }
