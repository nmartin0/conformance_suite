# diagnostics are written to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() { command sed "$@" 2>&1; }
