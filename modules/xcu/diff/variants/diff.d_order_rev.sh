# a conforming diff that reports the directory entries in reverse order
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because the text fixes no order of the entries of a directory comparison.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref d_order_rev "$@"; }
