# a trailing slash gives status 1 and no diagnostic
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ ! -d "$2" ] && case $2 in */) true ;; *) false ;; esac; then exit 1; fi; command mv "$@" )
