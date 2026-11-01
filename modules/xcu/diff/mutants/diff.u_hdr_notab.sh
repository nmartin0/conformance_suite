# -u: the header uses a blank instead of a tab before the time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_hdr_notab "$@"; }
