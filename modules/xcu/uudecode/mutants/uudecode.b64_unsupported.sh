# uudecode rejects the Base64 format
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/uuref.sh"
uudecode() { pc_uuref b64_unsupported "$@"; }
pc_uu_install b64_unsupported
