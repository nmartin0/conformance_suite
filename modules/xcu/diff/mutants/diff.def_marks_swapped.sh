# default format: lines of file1 marked > and lines of file2 marked <
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref def_marks_swapped "$@"; }
