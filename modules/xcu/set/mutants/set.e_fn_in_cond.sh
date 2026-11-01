# under -e, a failure inside a function called as an if condition ends the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^pcf_body() { false;/pcf_body() { false || exit 1;/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
