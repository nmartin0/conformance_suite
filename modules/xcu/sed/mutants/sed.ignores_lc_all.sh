# sed ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() ( unset LC_ALL; command sed "$@" )
