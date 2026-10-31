# a directory source without -R is copied as with -R
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( for a; do [ -d "$a" ] && { command cp -R "$@"; exit; }; done; command cp "$@" )
