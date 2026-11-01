# a failure in the first command of an if condition list ends the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^\(pcf_lf() .*\)return 1/\1exit 1/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
