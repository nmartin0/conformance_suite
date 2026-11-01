# ln writes no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() { command ln "$@" 2>/dev/null; }
