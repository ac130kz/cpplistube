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

## References
```
https://stackoverflow.com/questions/14365875/qt-cannot-put-an-image-in-a-table
https://stackoverflow.com/questions/26958644/qt-loading-indicator-widget
https://stackoverflow.com/questions/48238544/qnetworkaccessmanager-no-such-signal
https://kangolin.wordpress.com/2017/06/26/working-with-http-in-qt
https://github.com/blackberry/Cascades-Samples/blob/master/httptest/src/PostHttp.cpp
https://doc.qt.io/qt-5/qtnetwork-http-httpwindow-cpp.html
```
