# m4: m4exit after an error always exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F m4exit_error_forces1 "$@"; }
