# true writes to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
true() { printf x >&2; return 0; }
