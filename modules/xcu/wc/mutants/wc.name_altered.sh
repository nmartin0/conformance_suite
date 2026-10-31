# the file name is written with a ./ in front
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/wcref.sh"
wc() { pc_wcref name_altered "$@"; }
