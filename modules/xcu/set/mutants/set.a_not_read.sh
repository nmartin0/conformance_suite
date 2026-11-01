# variables assigned by read are not exported under set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^read PCB <pc.rin$/set +a; read PCB <pc.rin; set -a/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
