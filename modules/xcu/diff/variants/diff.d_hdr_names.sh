# a conforming diff whose directory diff line gives file names, not pathnames
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the directory diff line is to carry file names (pathname components), not necessarily pathnames.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref d_hdr_names "$@"; }
