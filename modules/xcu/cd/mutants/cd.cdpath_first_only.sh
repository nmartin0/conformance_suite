# only the first CDPATH entry is searched
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cd() { CDPATH=${CDPATH%%:*} command cd "$@"; }
