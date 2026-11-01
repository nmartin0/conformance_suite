# csplit: an offset with an explicit plus sign is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/csplitref.sh"
csplit() { pc_csplitref plus_rejected "$@"; }
