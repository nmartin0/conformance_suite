# -b: leading white space is ignored altogether
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref b_lead_ignored "$@"; }
