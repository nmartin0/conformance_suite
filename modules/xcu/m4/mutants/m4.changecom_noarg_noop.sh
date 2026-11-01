# m4: leaves the comment strings alone when changecom has no arguments
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F changecom_noarg_noop "$@"; }
