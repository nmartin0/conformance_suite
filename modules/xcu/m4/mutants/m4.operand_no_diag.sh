# m4: writes no diagnostic for an operand that cannot be opened
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F operand_no_diag "$@"; }
