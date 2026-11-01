# an object with no symbols is reported with exit status 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref nosym_status1 "$@"; }
