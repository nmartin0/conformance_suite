# test: -ge behaves as -gt (in the standalone test only)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant int_ge_as_gt t
