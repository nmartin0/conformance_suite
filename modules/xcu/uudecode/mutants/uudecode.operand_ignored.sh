# uudecode reads standard input even when a file operand is given
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref operand_ignored "$@"; }
pc_uu_install operand_ignored
