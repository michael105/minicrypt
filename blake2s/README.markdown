original readme.

Source: github.com/camerondm9/blake2s-micro

I modified the sources a bit, to spare some more space.

But wasn't that successful. It's been already wonderful squeezed.

(misc)

----


# BLAKE2s for embedded devices

This is a size-optimized implementation of the [BLAKE2s hash function](https://blake2.net/).
With all optimizations/constraints enabled, it can achieve a code size of about 500 bytes (600 bytes including constant tables).

**Note**: although this implementation is based on the [reference implementation](https://github.com/BLAKE2/BLAKE2/tree/master/ref)
and it has been tested on all relevant test vectors, I cannot guarantee it will
work in all cases as it has been modified quite a lot.

Things that are different from the original reference implementation:

  * Unrolled loops are re-rolled to save code size.
  * No support for salting, yet. This is something that would
    be nice to have, if it can be disabled by default.
  * Code relies on modern features of C, to make it easier to read. We don't
    live in the 90's, so we can use variable declarations directly in a `for`
    loop for example. This also means using `#pragma once` in header files.
    This is non-standard, but supported by
    [basically every compiler](https://en.wikipedia.org/wiki/Pragma_once#Portability).
  * Code has been reformatted, mostly following the
[MicroPython coding conventions](https://github.com/micropython/micropython/blob/master/CODECONVENTIONS.md#c-code-conventions).

The original license of the reference source code has been kept, so you can use
it under the CC0 1.0, OpenSSL, or Apache 2.0 license.


## Usage

Usage is simple:

```c
#include "blake2s.h"

// this is the message to be hashed
const uint8_t data[BLAKE2S_BLOCKBYTES] = {0, 1, 2, 3};

void main() {
    uint8_t result[32];

    // hash the data
    blake2s_state S;
    blake2s_init(&S);
    blake2s_update(&S, data, sizeof(data));
    blake2s_final(&S, &result);

    // do something with the result
    for (size_t i = 0; i < sizeof(result); i++) {
        printf("%02x", result[i]);
    }
    printf("\n");
}
```


## Performance

The primary goal of this library is small size, but I've also done some performance testing.

| System    | Optimization level | code size | constant data size | kB/s         |
| --------- | ------------------ | --------- | ------------------ | ------------ |
| Cortex-M0 | `gcc -Os`          | 466 bytes | 112 bytes          | ?            |
| Cortex-M4 | `gcc -Os`          | 498 bytes | 112 bytes          | ?            |

Minimum code size is listed (all options disabled and a fixed output length).
Code size includes stdlib dependencies (`memset` and `memcpy`, roughly 32 bytes). These aren't necessary if your project already has them.

Computing a hash also requires allocating a writable `blake2s_state` structure, which is 112 bytes in size.


## Options

There are a few options in blake2s.h with feature/codesize tradeoffs:

| Option              | Effect                                                                                        |
| ------------------- | --------------------------------------------------------------------------------------------- |
| `BLAKE2S_OUTLEN`    | The digest length, usually 32. If set to 0, the functions will require an `outlen` parameter. |
| `BLAKE2S_64BIT`     | Allow the total length of the hashed data to be larger than 4GB.                              |
| `BLAKE2S_KEYED`     | Enable support for keyed initialization of the hash.                                          |


## TODO

  * Check against all applicable test vectors.
  * Expand API.
