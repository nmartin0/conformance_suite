# a conforming diff that also reports common subdirectories
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed because common subdirectories may be reported (Diff Directory Comparison Format).
. "${PC_MUTANT_FILE%/*}/../mutants/diffref.sh"
diff() { pc_diffref d_common_subdirs "$@"; }
