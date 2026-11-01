# a note is written to standard output
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( command mv "$@" || exit; echo "moved" )
