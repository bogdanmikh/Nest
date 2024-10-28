# GLFM build tests

## Automated tests

The [build.yml](../.github/workflows/build.yml) GitHub Action builds GLFM automatically for all target platforms and arc
hitectures. Builds fail on compilation warnings or analyzer warnings.

The [build_examples.yml](../.github/workflows/build_examples.yml) GitHub Action builds GLFM examples automatically.
Builds fail if deprecated functions are used.

## Manual tests

The scripts in this directory are similar to the GitHub Actions. The scripts work on Linux, macOS, and Windows (tested
with git-bash/MINGW64). CMake is required.

When running `build_all.sh`, GLFM is conditionally built for each target platform based on the tools installed:

* Apple platforms: Xcode is installed (macOS only).
* Emscripten: emsdk is installed (`emcmake` is in the path).
* Android: Android NDK 17 or newer is installed (`ANDROID_NDK_HOME` environment variable is set).

On macOS, `ANDROID_NDK_HOME` is something like "~/Library/Android/sdk/ndk/23.2.8568313".

## Analyzing with clang-tidy

The build scripts run `clang-tidy` if it is available.

The [clang-tidy-analyze.yml](clang-tidy-analyze.yml) file includes the parent [.clang-tidy](../.clang-tidy) checks and
adds analyzer checks. Those extra analyzer checks take a bit longer to run.

The current list of checks in [.clang-tidy](../.clang-tidy) will likely change in the future.

For Apple platforms ([build_apple.sh](build_apple.sh)), the process of extracting info from `xcodebuild` and sending it
to `clang-tidy` is a bit fragile and will likely break in the future. As of now, it works with Xcode 14.3 and clang-tidy
16.

### Installing clang-tidy

For Android targets, the Android NDK has a built in `clang-tidy`.

For Emscripten targets on Ubuntu, install clang-tidy:

```
sudo apt install clang-tidy
```

For Apple targets, both `xcpretty` and `clang-tidy` are required:

```
brew install xcpretty llvm
```

