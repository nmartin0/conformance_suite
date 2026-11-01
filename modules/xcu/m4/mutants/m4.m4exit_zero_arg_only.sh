# m4: m4exit with a zero argument after an error exits 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F m4exit_zero_arg_only "$@"; }
