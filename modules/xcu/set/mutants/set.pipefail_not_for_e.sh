# under -e the pipefail status of a pipeline is not used
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^(exit 6) | true$/(exit 6) | true || :/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
