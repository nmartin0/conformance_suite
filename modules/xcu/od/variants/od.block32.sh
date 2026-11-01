# thirty-two bytes of a one-byte type per line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: The block size is any multiple of the least common multiple of the type sizes (EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref block32 "$@"; }
