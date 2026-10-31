# tr writes no diagnostic when it cannot write
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
tr() { command tr "$@" 2>/dev/null; }
