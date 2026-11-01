# mv ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( unset LC_COLLATE; command mv "$@" )
