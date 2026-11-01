# a conforming diff that separates the fields of -c header and range lines by several blanks
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because a plain blank in the -c header and range formats stands for one or more blanks (Diff -c Output Format).
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref c_blanks "$@"; }
