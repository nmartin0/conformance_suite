# recursive removal empties the directory but keeps it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm R_leaves_top "$@" )
