# cp writes a word to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
cp() ( command cp "$@"; rc=$?; echo copied; exit "$rc" )
