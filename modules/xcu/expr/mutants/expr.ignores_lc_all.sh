# expr ignores LC_ALL
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
expr() ( unset LC_ALL; command expr "$@" )
