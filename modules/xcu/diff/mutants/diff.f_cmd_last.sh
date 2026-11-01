# -f: the command letter comes after the numbers
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref f_cmd_last "$@"; }
