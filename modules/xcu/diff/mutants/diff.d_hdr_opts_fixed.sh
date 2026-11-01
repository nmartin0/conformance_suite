# directories: the diff line shows other options than those given
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref d_hdr_opts_fixed "$@"; }
