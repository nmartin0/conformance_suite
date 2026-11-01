# a failure inside a brace group does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^{ false; echo after; }$/{ set +e; false; echo after; }/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
