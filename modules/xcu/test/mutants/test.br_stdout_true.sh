# test: a true result writes to standard output (in the standalone [ only)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant stdout_true b
