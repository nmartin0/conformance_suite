# a symbolic link to a file is removed together with its target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm link_target_removed "$@" )
