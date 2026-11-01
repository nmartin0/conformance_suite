# set -f leaves the ? pattern character active
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo pc.f?$/set +f; echo pc.f?; set -f/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
