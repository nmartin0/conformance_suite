# -c: the file2 range line ends in three hyphens
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref c_range2_dashes "$@"; }
