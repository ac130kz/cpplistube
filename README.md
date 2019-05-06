# cpplistube
Qt Youtube playlist viewer. Just paste your Youtube v3 API key into the `config.ini` near the executable.

## Building
Requirements:
* Compiler with C++17 support
* Qt
* cmake

```bash
mkdir build && cd build && cmake .. && make -j4
```

## TODO
* Thumbnail loading
* Full playlist loading (only 50 entries at the moment)
* Editing features
