# under -e, a failure inside a brace group before || ends the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^{ false; echo reached3; }/{ false || exit 1; echo reached3; }/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
