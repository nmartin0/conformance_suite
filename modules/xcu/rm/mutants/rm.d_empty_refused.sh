# -d refuses an empty directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm d_empty_refused "$@" )
