# the diagnostic for an unreadable file goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref missing_diag_stdout any "$@"; }
