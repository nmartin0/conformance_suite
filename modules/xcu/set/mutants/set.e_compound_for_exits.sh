# a for command with the status of an ignored failure ends the shell under -e
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^for pci in 1; do false && true; done$/for pci in 1; do false \&\& true; done || exit $?/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
