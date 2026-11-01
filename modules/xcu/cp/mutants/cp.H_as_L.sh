# with -H links found in the hierarchy are followed too
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( n=$#; for a; do [ "$a" = -H ] && a=-L; set -- "$@" "$a"; done; shift "$n"; command cp "$@" )
