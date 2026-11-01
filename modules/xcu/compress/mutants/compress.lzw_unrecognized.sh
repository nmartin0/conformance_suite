# -d -c does not recognise lzw data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm lzw_unrecognized "$@"; }
