# test: -e is false for a directory (as a shell function only)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant e_dir_false f
