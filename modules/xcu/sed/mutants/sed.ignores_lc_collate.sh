# sed ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() ( unset LC_COLLATE; command sed "$@" )
