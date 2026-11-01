# -u: context lines are written without the leading space
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_ctx_nospace "$@"; }
