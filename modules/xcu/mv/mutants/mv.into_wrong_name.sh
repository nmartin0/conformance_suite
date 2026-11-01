# the moved files get a suffix in the directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( d=; for a; do d=$a; done; if [ $# -gt 2 ] && [ -d "$d" ]; then n=$#; i=0; for s; do i=$((i + 1)); [ "$i" -lt "$n" ] || break; command mv "$s" "$d/${s##*/}.moved" || exit 1; done; exit 0; fi; command mv "$@" )
