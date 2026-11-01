# a conforming diff that writes an empty unaffected line of -u output as an empty line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because it is implementation-defined whether an empty unaffected line is an empty line or one space (Diff -u Output Format).
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref u_empty_blank "$@"; }
