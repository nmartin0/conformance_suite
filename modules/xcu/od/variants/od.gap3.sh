# items and offsets are separated by three blanks
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: Items and offsets are separated by one or more blanks, the count unspecified (EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref gap3 "$@"; }
