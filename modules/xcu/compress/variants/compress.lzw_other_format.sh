# A whole conforming system whose default algorithm stores data in another
# format (the format of the lzw algorithm is implementation-defined): data
# written under the suffix .Z is deflate-form, -b is accepted and ignored
# for it, and decompressing a .Z file reads that format.
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/cmut.sh"
compress() { pc_cm lzw_other_format "$@"; }
