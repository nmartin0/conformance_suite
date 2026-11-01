# mv ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( unset LANG; command mv "$@" )
