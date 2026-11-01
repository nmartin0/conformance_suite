# diagnostic for dot goes to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm dot_diag_stdout "$@" )
