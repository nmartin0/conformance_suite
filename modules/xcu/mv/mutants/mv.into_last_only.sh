# only the last source is moved into the directory
# Copyright (c) 2026 Nicholas Martin. All rights reserved.
mv() ( s=; d=; for a; do s=$d; d=$a; done; if [ $# -gt 2 ] && [ -d "$d" ]; then command mv "$s" "$d"; else command mv "$@"; fi )
