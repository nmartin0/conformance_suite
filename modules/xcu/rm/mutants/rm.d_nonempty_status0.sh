# -d on a non-empty directory exits 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm d_nonempty_status0 "$@" )
