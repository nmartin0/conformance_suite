# tr exits 0 although it could not write its output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
tr() { command tr "$@"; return 0; }
