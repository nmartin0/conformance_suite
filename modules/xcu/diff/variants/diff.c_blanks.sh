# a conforming diff that separates the fields of -c header and range lines by several blanks
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref c_blanks "$@"; }
