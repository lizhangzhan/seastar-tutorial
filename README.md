### 0. System Requirements

 * OS: ubuntu22.04(x86_64/aarch64)
 * Compiler: clang-18
 * BuildTools: bazel-5.0.0+

### 1. Denpendencies

  Compile and install `seastar` according to [install instruction](https://github.com/scylladb/seastar/blob/master/HACKING.md)

### 2. Build

```shell
  $ export PATH=${PATH}:.
  $ pybazel build hello_world
```

