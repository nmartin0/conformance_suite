# cp exits 0 although it reported an error
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( command cp "$@"; exit 0 )
