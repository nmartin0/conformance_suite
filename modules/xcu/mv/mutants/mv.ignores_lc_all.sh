# mv ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( unset LC_ALL; command mv "$@" )
