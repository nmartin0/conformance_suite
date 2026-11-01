# -d removes a non-empty directory with its contents
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm d_nonempty_removed "$@" )
