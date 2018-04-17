# **BIMO** - **BI**SECT **M**edia C**o**re Library

## Build

### Setup conan remotes

```
> conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
```

### Windows
```
> md build
> cd build
> cmake .. -DBIMO_BUILD_TESTS=1 -G "Visual Studio 15 2017 Win64"
> cmake --build .
```

Open the solution file build\bimo.sln
