# ln reports failure after a successful link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() { command ln "$@"; pc_rc=$?; if [ "$pc_rc" -eq 0 ]; then return 1; fi; return "$pc_rc"; }
