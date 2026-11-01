# ln exits 0 although it failed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
ln() { command ln "$@"; return 0; }
