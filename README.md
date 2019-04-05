## base16 - A wicked fast RFC4648-conformed Base16 encoder/decoder

This Base16 implementation presents a way to optimize base16 encode/decode in linear facet(not significant) without intervention of SIMD instructions, provides a simple example of how `.data` segment cache can boost up data lookup.

# Encode APIs

There're three encode methods:

```c
void nano_base16_encode_baseline(char * restrict, const void * restrict, size_t);

void nano_base16_encode(char * restrict, const void * restrict, size_t);

void nano_base16_encode2(char * restrict, const void * restrict, size_t);
```

### Which API should I choose for encode?

TL;DR. `nano_base16_encode`

`nano_base16_encode_baseline` uses definition-intuitive way for encoding.

`nano_base16_encode` which encoding in a manner of taking a byte as a whole, which each time produce two encoded characters in an atomic way, with negligible(1kb) `.data` segment overhead.

`nano_base16_encode2` encode in a manner of taking two bytes as a whole, which each time produce four encoded characters in an atomic way, with relative big `.data` segment overhead(around 512kb).

# Decode APIs

TODO

# Test

`test.c` provides all possible tests for Base16 encode/decode.

```shell
$ make [ debug | release ]
$ ./test-debug
```

# Benchmark

Benchmark machine:

```
Intel(R) Core(TM) i5-5287U CPU @ 2.90GHz (Max 3.30 GHz, 2 Cores, 4 Threads)
16 GB 1867 MHz DDR3 RAM
```

Without compiler level optimization:

```shell
$ make [ debug ]
$ ./test-debug

Benchmarking  size: 1073741824
Time elapsed: 19.2773s	nano_base16_encode_baseline()
Time elapsed: 2.45246s	nano_base16_encode()
Time elapsed: 1.40033s	nano_base16_encode2()

Benchmarking  size: 3221225472
Time elapsed: 46.7634s	nano_base16_encode_baseline()
Time elapsed: 7.32601s	nano_base16_encode()
Time elapsed: 4.20426s	nano_base16_encode2()
```

With `-O2` compiler level optimization:

```shell
$ make release
$ ./test

Benchmarking  size: 1073741824
Time elapsed: 1.89828s	nano_base16_encode_baseline()
Time elapsed: 0.5519s	nano_base16_encode()
Time elapsed: 0.332284s	nano_base16_encode2()

Benchmarking  size: 3221225472
Time elapsed: 5.59842s	nano_base16_encode_baseline()
Time elapsed: 1.66608s	nano_base16_encode()
Time elapsed: 0.993617s	nano_base16_encode2()
```

Above result reveal that the `nano_base16_encode` is a good in-production choice between speed and `.data` segment cache overhead.

# TODO

* Run tests on big endian machine.

* Make this header compatible with C++; Platform porting.

# *References*

[The Base16, Base32, and Base64 Data Encodings](https://tools.ietf.org/html/rfc4648)

[libbase64 - A simple libbase64 library](https://github.com/gozfree/gear-lib/tree/master/libbase64)

[java base 16 编码和解码](https://gist.github.com/turbidsoul/5226998)
