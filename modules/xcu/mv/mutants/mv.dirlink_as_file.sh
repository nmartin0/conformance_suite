# a link to a directory as last operand is replaced by the moved file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -h "$2" ] && [ -d "$2" ]; then rm -f "$2"; command mv "$1" "$2"; exit; fi; command mv "$@" )
