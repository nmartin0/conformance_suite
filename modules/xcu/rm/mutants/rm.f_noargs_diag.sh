# rm -f with no operand writes a usage diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm f_noargs_diag "$@" )
