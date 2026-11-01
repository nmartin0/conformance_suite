# -A n ends with an empty line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: With -A n it is unspecified whether the final-offset line is written as an empty line (EXTENDED DESCRIPTION).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref an_empty "$@"; }
