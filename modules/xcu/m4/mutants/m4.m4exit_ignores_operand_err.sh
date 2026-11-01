# m4: m4exit(0) forgets an unopened operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F m4exit_ignores_operand_err "$@"; }
