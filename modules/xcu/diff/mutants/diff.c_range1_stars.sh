# -c: the file1 range line ends in three stars
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref c_range1_stars "$@"; }
