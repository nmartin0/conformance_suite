# refusing to overwrite changes the existing file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm refuse_old_appended "$@"; }
