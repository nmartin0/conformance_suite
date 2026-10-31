# with -P a link in the hierarchy is followed
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( n=$#; for a; do [ "$a" = -P ] && a=-L; set -- "$@" "$a"; done; shift "$n"; command cp "$@" )
