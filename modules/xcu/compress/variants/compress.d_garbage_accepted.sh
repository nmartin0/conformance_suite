# Data that is not compressed data may be accepted: the standard does not
# require decompression of such a file to fail, so a system may pass it
# through unchanged as a successful result.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/cmut.sh"
compress() { pc_cm d_garbage_accepted "$@"; }
