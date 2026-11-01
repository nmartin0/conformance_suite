# the -v message has two lines
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
# Allowed: the -v message has an unspecified format (STDOUT)
rm() { . "${PC_MUTANT_FILE%/*}/../mutants/rmref.sh"; pc_rm v_two_lines "$@"; }
