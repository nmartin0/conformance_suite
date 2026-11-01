# a trailing slash gives a diagnostic, status 0, nothing moved
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ ! -d "$2" ] && case $2 in */) true ;; *) false ;; esac; then echo "mv: bad target" >&2; exit 0; fi; command mv "$@" )
