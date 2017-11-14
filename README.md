# mkfifo-example

Simple example of processes communicating over a named pipe.

## Building

```
make mkfifo-example
```

## Running

```
./mkfifo-example creator fifo
./mkfifo-example writer fifo &
./mkfifo-example reader fifo
```

Notice reader or writer will block until its counterpart is running.

