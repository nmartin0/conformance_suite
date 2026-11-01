# the long name errexit acts like noexec
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^set \([-+]\)o errexit$/set \1o noexec/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
