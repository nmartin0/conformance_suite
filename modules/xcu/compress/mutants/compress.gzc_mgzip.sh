# -m gzip writes lzw data under the .gz name
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm gzc_mgzip "$@"; }
