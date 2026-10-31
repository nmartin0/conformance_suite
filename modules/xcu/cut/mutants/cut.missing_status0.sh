# an unreadable file operand leaves the status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cutref.sh"
cut() { pc_cutref missing_status0 any "$@"; }
