# expr ignores LC_COLLATE
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
expr() ( unset LC_COLLATE; command expr "$@" )
