# uudecode rejects a blank line in Base64 data
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref b64_blank_error "$@"; }
pc_uu_install b64_blank_error
