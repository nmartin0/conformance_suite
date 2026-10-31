# with -L a link in the hierarchy is copied as a link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( n=$#; for a; do [ "$a" = -L ] && a=-P; set -- "$@" "$a"; done; shift "$n"; command cp "$@" )
