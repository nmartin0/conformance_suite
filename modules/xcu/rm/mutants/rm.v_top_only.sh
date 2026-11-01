# -v reports only the operands, not what a hierarchy held
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm v_top_only "$@" )
