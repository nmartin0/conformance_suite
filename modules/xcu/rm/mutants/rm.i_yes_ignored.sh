# -i keeps the file after an affirmative reply
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm i_yes_ignored "$@" )
