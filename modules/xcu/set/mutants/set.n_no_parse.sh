# set -n makes the shell stop reading, so later syntax errors go unseen
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='/^set -n$/q'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
