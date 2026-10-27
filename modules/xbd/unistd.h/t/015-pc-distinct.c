/*
 * _PC_ constants differ.
 * Copyright (c) 2026 Nicholas Martin. All rights reserved.
 *
 * ASSERTION XBD.unistd.h.015
 */
#include <string.h>
#include <unistd.h>
#include "pc.h"

int main(void)
{
	static const struct { const char *name; long val; } c[] = {
		{ "_PC_2_SYMLINKS", (long)_PC_2_SYMLINKS },
		{ "_PC_ALLOC_SIZE_MIN", (long)_PC_ALLOC_SIZE_MIN },
		{ "_PC_ASYNC_IO", (long)_PC_ASYNC_IO },
		{ "_PC_CHOWN_RESTRICTED", (long)_PC_CHOWN_RESTRICTED },
		{ "_PC_FILESIZEBITS", (long)_PC_FILESIZEBITS },
		{ "_PC_LINK_MAX", (long)_PC_LINK_MAX },
		{ "_PC_MAX_CANON", (long)_PC_MAX_CANON },
		{ "_PC_MAX_INPUT", (long)_PC_MAX_INPUT },
		{ "_PC_NAME_MAX", (long)_PC_NAME_MAX },
		{ "_PC_NO_TRUNC", (long)_PC_NO_TRUNC },
		{ "_PC_PATH_MAX", (long)_PC_PATH_MAX },
		{ "_PC_PIPE_BUF", (long)_PC_PIPE_BUF },
		{ "_PC_PRIO_IO", (long)_PC_PRIO_IO },
		{ "_PC_REC_INCR_XFER_SIZE", (long)_PC_REC_INCR_XFER_SIZE },
		{ "_PC_REC_MAX_XFER_SIZE", (long)_PC_REC_MAX_XFER_SIZE },
		{ "_PC_REC_MIN_XFER_SIZE", (long)_PC_REC_MIN_XFER_SIZE },
		{ "_PC_REC_XFER_ALIGN", (long)_PC_REC_XFER_ALIGN },
		{ "_PC_SYMLINK_MAX", (long)_PC_SYMLINK_MAX },
		{ "_PC_SYNC_IO", (long)_PC_SYNC_IO },
		{ "_PC_TIMESTAMP_RESOLUTION", (long)_PC_TIMESTAMP_RESOLUTION },
		{ "_PC_VDISABLE", (long)_PC_VDISABLE },
	};
	size_t i, j, n = sizeof c / sizeof c[0];

	pc_begin("XBD.unistd.h.015");
	pc_test();
	for (i = 0; i < n; i++)
		for (j = i + 1; j < n; j++) {
			int ok = 0;
			pc_check(ok || c[i].val != c[j].val, "%s and %s have the same value (%ld)", c[i].name, c[j].name, c[i].val);
		}
	pc_pass();
}
