# a moved link is made to refer to the absolute path of its target
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -h "$1" ] && [ ! -d "$2" ]; then t=$(ls -ld "$1"); t=${t##* -> }; rm -f "$1"; ln -s "$PWD/$t" "$2"; exit; fi; command mv "$@" )
