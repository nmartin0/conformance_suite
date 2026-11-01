# -u: the zone is written with a colon
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_hdr_zone_colon "$@"; }
