# test: -z of a non-empty string is true
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant z_nonempty_true ftb
