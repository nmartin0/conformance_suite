# m4: scans the defining text apart from the input that follows it
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
. "${PC_MUTANT_FILE%/*}/m4ref.sh"
m4() { pc_m4ref F rescan_isolated "$@"; }
