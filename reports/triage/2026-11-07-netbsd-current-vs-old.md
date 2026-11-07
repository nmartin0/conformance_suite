# NetBSD 11.0, current tree vs the 2026-10-29 run

Run: reports/2026-11-07-netbsd-amd64-current (root 8983 assertions, user 9011).
The old run used an older tree (8856 root assertions); 155 IDs are new, 30 are gone.
The guest ran with QEMU TCG, one CPU, `-cpu qemu64` (the old run used the default
CPU and multi-threaded TCG). Sessions 1-4 ran on a results disk that was rebuilt after
an unclean stop; its contents were copied over unchanged.

Root phase, assertions present in both runs, outcome changed:
- UNINITIATED to NOTINUSE 193, UNINITIATED to PASS 29: toolchain and option labelling fixes.
- FAIL to PASS 25, UNRESOLVED to PASS 12, NOTINUSE to PASS 5.
- PASS to FAIL 23, PASS to UNRESOLVED 1 (list below). Not yet examined: some are
  likely test changes since the old tree, some may be timing noise in the slower VM
  (setsockopt.013 gave up at 198 ms against a 200 ms bound, timer_getoverrun.008).
  They need a repeat run before being counted as regressions.

PASS to FAIL/UNRESOLVED: XCU.alias.003 alias.007 chmod.006 cp.013 csplit.007 find.016
grep.008 join.005 join.009 ls.042 m4.004 m4.012 od.002 patch.006 ps.006 ps.007 set.012
test.008 touch.007 tr.008 xargs.008; XSH.fpathconf.007 (UNRESOLVED), setsockopt.013,
timer_getoverrun.008.

Guest note: the NetBSD kernel panics inside its virtio flush on `halt -p` after the
disks are synced; results are unaffected.
