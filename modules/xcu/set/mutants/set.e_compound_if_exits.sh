# an if command with the status of an ignored failure ends the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^if true; then false && true; fi$/if true; then false \&\& true; fi || exit $?/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
