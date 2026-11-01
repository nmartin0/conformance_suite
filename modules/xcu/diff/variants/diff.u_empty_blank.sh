# a conforming diff that writes an empty unaffected line of -u output as an empty line
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref u_empty_blank "$@"; }
