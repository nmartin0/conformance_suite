# a failing command substitution in an assignment does not end the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^pcv=\$(exit 3)$/pcv=$(exit 3) || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
