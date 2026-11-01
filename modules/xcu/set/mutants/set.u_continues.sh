# a message is written for an unset parameter but the shell goes on
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^echo "x:\$PCU1"$/echo "pcm: PCU1: parameter not set" >\&2; echo "x:"/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
