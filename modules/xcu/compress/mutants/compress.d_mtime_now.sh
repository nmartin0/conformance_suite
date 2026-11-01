# the decompressed file gets the current time as modification time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm d_mtime_now "$@"; }
