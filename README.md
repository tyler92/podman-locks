# The small tool for debugging deadlocks in Podman
Just run this tool without any arguments and observer output like this:

```sh
Open libpod_rootless_lock_1000 ...
Lock #4 owner is thread 1796951
Lock #5 owner is thread 1796951
Lock #8 owner is thread 1797100
```

It allows to understand which mutexes are locked by particular threads and might help with deadlocks debugging.