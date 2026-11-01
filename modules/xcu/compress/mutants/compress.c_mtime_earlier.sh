# the output file is a minute older than the input file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm c_mtime_earlier "$@"; }
