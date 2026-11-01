# the refused > reports success
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^if echo two >pc.nc1; then/if echo two >\/dev\/null; then/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
