# variables assigned by a parameter expansion are not exported under set -a
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^: \${PCC:=3}$/set +a; : ${PCC:=3}; set -a/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
