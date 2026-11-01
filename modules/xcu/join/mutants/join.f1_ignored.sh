# -1 is ignored (file1 is joined on its first field)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/joinref.sh"
join() { pc_joinref f1_ignored "$@"; }
