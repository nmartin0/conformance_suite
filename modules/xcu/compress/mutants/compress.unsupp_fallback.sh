# an unsupported algorithm is replaced by lzw
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm unsupp_fallback "$@"; }
