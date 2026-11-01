# with -H a link given as an operand is not followed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( n=$#; for a; do [ "$a" = -H ] && a=-P; set -- "$@" "$a"; done; shift "$n"; command cp "$@" )
