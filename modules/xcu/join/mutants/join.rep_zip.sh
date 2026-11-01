# repeated keys are paired line for line, not in all combinations
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref rep_zip "$@"; }
