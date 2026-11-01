# -m deflate names the output with the suffix .Z
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm gzs_mdeflate "$@"; }
