# an existing output file is overwritten without -f
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/cmut.sh"
compress() { pc_cm overwrite_silently "$@"; }
