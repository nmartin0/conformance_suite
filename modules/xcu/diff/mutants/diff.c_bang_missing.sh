# -c: changed lines are not marked
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref c_bang_missing "$@"; }
