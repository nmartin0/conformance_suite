# ln writes to standard output on success
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() { command ln "$@"; pc_rc=$?; if [ "$pc_rc" -eq 0 ]; then printf 'linked\n'; fi; return "$pc_rc"; }
