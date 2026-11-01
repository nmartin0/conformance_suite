# a first operand - is taken as a file called -
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/diffref.sh"
diff() { pc_diffref s_dash_f1_broken "$@"; }
