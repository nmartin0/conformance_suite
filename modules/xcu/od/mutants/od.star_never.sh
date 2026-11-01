# repeated lines are never replaced by an asterisk
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/odref.sh"
od() { pc_odref star_never "$@"; }
