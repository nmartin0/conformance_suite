# directories: Only in joins the directory and the file with a slash
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref d_only_slash "$@"; }
