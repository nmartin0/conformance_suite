# the -v message quotes the pathname
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the -v message has an unspecified format that contains the pathname (STDOUT)
rm() { . "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"; pc_rm v_quoted "$@"; }
