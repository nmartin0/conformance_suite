# -t a names byte 10 lf instead of nl
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
#
# Allowed because: The table of names lets byte 10 be written lf or nl (EXTENDED DESCRIPTION, type a).
. "${PC_MUTANT_FILE%/*}/../mutants/odref.sh"
od() { pc_odref lf_name "$@"; }
