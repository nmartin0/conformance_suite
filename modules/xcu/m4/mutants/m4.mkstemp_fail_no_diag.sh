# m4: a failed mkstemp writes no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F mkstemp_fail_no_diag "$@"; }
