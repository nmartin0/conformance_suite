# mv ignores LC_MESSAGES
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( unset LC_MESSAGES; command mv "$@" )
