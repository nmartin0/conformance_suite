# the trailing slash is dropped and the file is moved
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ ! -d "$2" ] && case $2 in */) true ;; *) false ;; esac; then command mv "$1" "${2%/}"; exit; fi; command mv "$@" )
