# with -a or -v an unreadable file is taken as empty, status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref missing_a_ok "$@"; }
