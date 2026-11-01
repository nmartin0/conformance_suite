# under -e, a failure inside a function called before || ends the shell
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
PCM_SED='s/^pcf_body2() { false;/pcf_body2() { false || exit 1;/'
. "${PC_MUTANT_FILE%/*/*}/mutants/_lib.sh"
