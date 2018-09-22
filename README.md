# mgcoro

`mgcoro` is an asymmetric stackless coroutine library, yup it is just grapped
from `boost::asio::coroutine`.  Not production-ready.

The **m**a**g**ic of this **coro**utine library is pedantically originated from
the Duff's device and pseudo-keywords from boost, which brings you the extremely
low overhead (a single `int`) and makes you look like a cool kid.

## Usage

The simple test snippet `test.cc` includes nearly all the examples you want.

```bash
$ cat test.cc
```

- `mgcoro` provides 3 major coroutine mechanisms:
    + `resumable`, the resumable scope indicator that enables the use of
    `yield` and `fork`
    + `yield`, you get it
    + `fork`, like forking a process

## License

MIT
