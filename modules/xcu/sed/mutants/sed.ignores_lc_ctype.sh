# sed ignores LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() ( unset LC_CTYPE; command sed "$@" )
