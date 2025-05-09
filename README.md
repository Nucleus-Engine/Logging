# Logging
Generic logging library designed for use in the Nucleus Engine

# Running Example
This example has one log message at the INFO level and 1,000 log messages at the ERROR level.
If you build at the WARN level, the INFO level message (which will be the first log message)
will not display, but the 1,000 logs at the ERROR level will display. Each of the ERROR messages
is logged by a spawned thread, but the INFO message is logged by the main thread before any of
those are spawned off. The order of the ERROR messages will be random because they delay by
an integer number of milliseconds between 0 and 100 when the thread spawns before submitting
the log to the log queue.

Cmake:
```
mkdir bld
cd bld
cmake .. -DLOG_LEVEL=WARN
make -j8
./example
```

Meson
```
meson setup build -Dlog_level=WARN
meson compile -C build
./build/examples/logger_example
```
