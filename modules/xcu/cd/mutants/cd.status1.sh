# cd changes the directory but exits 1
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { command cd "$@"; return 1; }
