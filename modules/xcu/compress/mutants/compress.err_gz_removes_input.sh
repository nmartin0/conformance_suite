# after an error on a gzip file the input file is removed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm err_gz_removes_input "$@"; }
