# only a bare dot or dot-dot is refused, not a trailing component
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
rm() ( . "${PC_MUTANT_FILE%/*}/rmref.sh"; pc_rm dot_trailing_unchecked "$@" )
