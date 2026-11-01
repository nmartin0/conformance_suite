# m4: a failed mkstemp leaves the status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F mkstemp_fail_status0 "$@"; }
