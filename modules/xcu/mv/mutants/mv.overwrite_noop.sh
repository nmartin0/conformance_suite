# an existing target is left alone, status 0
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -e "$2" ]; then exit 0; fi; command mv "$@" )
