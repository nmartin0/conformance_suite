# CDPATH is not consulted
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { CDPATH= command cd "$@"; }
