# > through a symbolic link to a regular file is not refused
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^if echo x >pc.ln; then/set +C; if echo x >pc.ln; then/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
