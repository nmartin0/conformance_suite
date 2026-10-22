# false writes to standard error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
false() { printf x >&2; return 1; }
