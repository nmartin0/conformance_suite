# hexadecimal digits are capitals
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: The letter case of hexadecimal digits is not fixed by the page.
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref hex_upper "$@"; }
