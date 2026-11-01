# test: -f does not follow links (as a shell function only)
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/testref.sh"
pcT_plant f_nofollow f
