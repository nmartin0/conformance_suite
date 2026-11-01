# -u: standard input is named stdin in the header instead of -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref u_stdin_name "$@"; }
