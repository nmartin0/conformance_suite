# rm also removes one-letter files that were not named
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm extra_removed "$@" )
