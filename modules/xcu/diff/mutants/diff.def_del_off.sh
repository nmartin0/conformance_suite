# default format: the file2 number of a deletion is one too high
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref def_del_off "$@"; }
