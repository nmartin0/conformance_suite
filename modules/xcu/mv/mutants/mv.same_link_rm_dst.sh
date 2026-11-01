# the destination is removed when it is another name of the source file
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ "$1" -ef "$2" ] && [ "${1#./}" != "${2#./}" ]; then rm -f "$2"; exit 0; fi; command mv "$@" )
