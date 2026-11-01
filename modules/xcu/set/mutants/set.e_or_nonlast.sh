# a failing command before || ends the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^\(pcf_or() .*\)return 1/\1exit 1/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
