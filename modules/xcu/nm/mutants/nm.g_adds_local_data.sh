# -g also lists a static variable but not a static function
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref g_adds_local_data "$@"; }
