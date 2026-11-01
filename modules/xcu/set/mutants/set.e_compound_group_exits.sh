# a brace group with the status of an ignored failure ends the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^{ false && true; }$/{ false \&\& true; } || exit $?/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
