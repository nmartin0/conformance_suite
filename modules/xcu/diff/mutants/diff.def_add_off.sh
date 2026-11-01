# default format: the file1 number of an addition is one too high
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref def_add_off "$@"; }
