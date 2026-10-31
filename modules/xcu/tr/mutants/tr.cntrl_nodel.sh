# [:cntrl:] lacks the delete character
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref cntrl_nodel "$@"; }
