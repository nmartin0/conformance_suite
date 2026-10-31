# sort -c writes the lines to standard output on disorder
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/sortref.sh"
sort() { pc_sortref c_prints "$@"; }
