# an existing target triggers a prompt although no -i was given and standard input is no terminal
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( if [ $# -eq 2 ] && [ -e "$2" ]; then printf "mv: overwrite %s? " "$2" >&2; read -r r; case $r in [yY]*) ;; *) exit 0 ;; esac; fi; command mv "$@" )
