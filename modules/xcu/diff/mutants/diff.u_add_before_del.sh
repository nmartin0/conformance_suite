# -u: added lines are written before the deleted lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_add_before_del "$@"; }
