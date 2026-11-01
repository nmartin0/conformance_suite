# the output file gets a very old modification time
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm c_mtime_epoch "$@"; }
