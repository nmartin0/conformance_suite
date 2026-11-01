# test: a true result writes to standard output (as a shell function only)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant stdout_true f
