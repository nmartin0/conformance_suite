# the -p option has no effect
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( n=$#; for a; do [ "$a" = -p ] || set -- "$@" "$a"; done; shift "$n"; command cp "$@" )
