# uudecode rejects a blank in Base64 data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref b64_space_error "$@"; }
pc_uu_install b64_space_error
