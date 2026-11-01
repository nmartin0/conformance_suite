# the file the link refers to is moved, not the link
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -h "$1" ] && [ ! -d "$2" ]; then t=$(ls -ld "$1"); t=${t##* -> }; command mv "$t" "$2"; exit; fi; command mv "$@" )
