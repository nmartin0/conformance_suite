# -u: deleted lines marked + and added lines marked -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_marks_swapped "$@"; }
