# default mode is 666 reduced by the mask (no search bits)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/mkdirref.sh"
mkdir() { pc_mkdirref default_666 "$@"; }
