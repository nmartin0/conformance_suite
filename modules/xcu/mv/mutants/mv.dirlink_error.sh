# a link to a directory as last operand is refused
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -h "$2" ] && [ -d "$2" ]; then echo "mv: $2 is not a directory" >&2; exit 1; fi; command mv "$@" )
