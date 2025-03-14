# Firush
Firush can help you find random manga/doujinshi/comics from [nHentai](https://nhentai.net/) based on search query.

The idea is to,
- Search the query
- Scrape the total page count _k_ 
- Pick a random number between 1 and _k_
- Scrape and return a random entry from that page.

Firush can be installed via Pypi:
```
pip install git+https://github.com/eeriemyxi/firush
```

> [!CAUTION]
> [Wheels](https://pythonwheels.com/) are currently unavailable. You will need a C compiler to install Firush. Wheels and Pypi upload is in the TODO list.

> [!CAUTION]
> Windows is not supported yet (but is planned). See [Technical Details](#technical-details) for more information.

> [!IMPORTANT] 
> Firush needs the user to have [curl](https://github.com/curl/curl) (tested on v8.12.1) installed with TLS (tested with [OpenSSL](https://github.com/openssl/openssl)) support.

Firush provides a web client via `firush web` command:
![](https://files.catbox.moe/lh6kwc.png)

Firush also provides a TUI client via `firush tui` command:
![](https://files.catbox.moe/wxbn6d.png)

> [!NOTE]
> You may also use Docker to use Firush, via `docker compose run firush firush --help`. But the TUI client won't be able to open your web browser (without some tweaking that I don't know how to do).

## Technical Details
The Python module is actually a C extension. The functions that do the base work are implemented in C and can be compiled to a shared library.

You can build the shared library with CMake:
```
mkdir build/
cd build/
cmake ..
make
```
That will build the shared library `build/lib/libfirush.so` (or `.dll` on Windows).

You should be able to make a wrapper for this library in any programming language that supports the C ABI for FFI.

Windows support currently is not available because the library depends on the POSIX `regex.h` header. Support is planned by making `pcre2`'s POSIX translation layer a requirement for the Windows build.
