# directories: Only in names the wrong directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref d_only_dir_wrong "$@"; }
