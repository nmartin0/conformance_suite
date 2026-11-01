# uudecode requires Base64 lines to hold whole groups of four
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref b64_quad_per_line "$@"; }
pc_uu_install b64_quad_per_line
