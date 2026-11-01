# mv ignores LC_CTYPE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( unset LC_CTYPE; command mv "$@" )
