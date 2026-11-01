# the file is moved although the status is 1 and a diagnostic written
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ ! -d "$2" ] && case $2 in */) true ;; *) false ;; esac; then command mv "$1" "${2%/}"; echo "mv: bad target" >&2; exit 1; fi; command mv "$@" )
