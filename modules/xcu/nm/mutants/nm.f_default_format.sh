# -f gives up the portable format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/nmref.sh"
nm() { pc_nmref f_default_format "$@"; }
