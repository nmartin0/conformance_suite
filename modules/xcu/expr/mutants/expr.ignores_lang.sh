# expr ignores LANG
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
expr() ( unset LANG; command expr "$@" )
