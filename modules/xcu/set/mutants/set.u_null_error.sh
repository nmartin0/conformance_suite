# under -u, a variable that is set but null is an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo "\[\$PCU3\]"$/echo "[${PCU3:?}]"/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
