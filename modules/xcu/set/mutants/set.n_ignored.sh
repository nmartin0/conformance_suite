# set -n has no effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set -n$//
s/^set -o noexec$//'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
