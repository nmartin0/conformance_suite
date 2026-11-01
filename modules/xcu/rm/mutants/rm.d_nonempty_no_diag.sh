# -d on a non-empty directory gives no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm d_nonempty_no_diag "$@" )
