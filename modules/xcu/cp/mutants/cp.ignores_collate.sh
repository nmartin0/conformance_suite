# cp ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( unset LC_COLLATE; command cp "$@" )
