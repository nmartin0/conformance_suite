# a conforming diff that writes an empty unaffected line of -u output as a single space
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed for the same reason as u_empty_blank, the other choice.
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref u_empty_space "$@"; }
