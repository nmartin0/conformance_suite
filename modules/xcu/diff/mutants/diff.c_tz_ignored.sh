# -c: the TZ variable is ignored, times are shown in UTC
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref c_tz_ignored "$@"; }
