# no diagnostic for a nonexistent operand
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm nonexist_silent "$@" )
