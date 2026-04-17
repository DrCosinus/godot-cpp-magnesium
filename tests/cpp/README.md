Run tests:

- Build and run tests using SCons (uses same environment as the extension build):

```powershell
scons test
```

- To only build the extension (unchanged):

```powershell
scons
```

Notes:
- Vendored doctest-like header is in `tests/vendor/doctest.h`.
- Tests are simple C++ programs built with the same flags/includes as the extension. If you encounter link errors, ensure godot-cpp submodule is initialized and the environment is configured as when building the extension.
