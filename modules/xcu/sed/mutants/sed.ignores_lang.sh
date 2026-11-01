# sed ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
sed() ( unset LANG; command sed "$@" )
