# tr writes a message to standard error although it succeeded
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/trref.sh"
tr() { pc_trref stderr_noise "$@"; }
