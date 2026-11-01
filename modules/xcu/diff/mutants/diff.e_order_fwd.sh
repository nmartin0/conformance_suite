# -e: the script runs from the start of the file to the end
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref e_order_fwd "$@"; }
