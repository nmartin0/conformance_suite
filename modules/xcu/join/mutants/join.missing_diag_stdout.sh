# the diagnostic for an unreadable file goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref missing_diag_stdout "$@"; }
