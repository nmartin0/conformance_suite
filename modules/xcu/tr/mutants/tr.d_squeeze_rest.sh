# tr -d also squeezes runs of the characters it keeps
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref d_squeeze_rest "$@"; }
