# recursive removal works with whole path names, which fail when they are too long
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm pathbased "$@" )
