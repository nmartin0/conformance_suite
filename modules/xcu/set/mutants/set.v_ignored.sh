# set -v has no effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -v$//
s/^set -o verbose$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
