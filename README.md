# ✨starlight ![ci-badge](https://github.com/InfiniteSynthesis/starlight/workflows/CI/badge.svg) ![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg) ![Ask Me Anything !](https://img.shields.io/badge/Ask%20me-anything-pink.svg)

![forthebadge](https://forthebadge.com/images/badges/made-with-c-plus-plus.svg)
![forthebadge](https://forthebadge.com/images/badges/powered-by-coffee.svg)


Starlight is a layout engine for cross-platform framework (e.g. [React Native](https://reactnative.dev/))

## Features🚀

Support all standard flex properties! Completely the same as [CSS Flexible Box Layout Standards](https://www.w3.org/TR/css-flexbox-1/).

## Testing 🔨

For testing we rely on [gtest](https://github.com/google/googletest) as a submodule. For any changes you make, you should ensure that all the tests are passing. In case you make any fixes or additions to the library please also add tests for that change to ensure we don't break anything in the future. Tests are located in the `layout_test` directory. You can run test under the following direction.

Instead of manually writing a test which ensures parity with web implementations of Flexbox you can run `layout_test/gentest.rb` to generate a test for you. You can write html which you want to verify in Starlight, in `layout_test/feature` folder, such as the following.

```html
<div id="my_test" style="width: 100px; height: 100px; align-items: center;">
  <div style="width: 50px; height: 50px;"></div>
</div>
```

## How to Use 🍕

1. install Ruby，cmake

2. install WebDriver

   ```shell
   $ sudo gem install watir
   $ sudo gem install watir-webdriver 
   ```

3. update chromedriver and add to PATH

   ```shell
   $ cd ./layout_test/
   $ ./updatechromedriver.sh # works under mac-os, otherwise please check the version and update manually
   $ export PATH="$PATH:`pwd`/Tools"
   ```

4. use Ruby to generate test files
    ``` shell
    $ ruby ./gentest.rb
    ```

5. compile and execute

    - on Mac: use bash script `layout_test/test-mac.sh` or seperate commands

        ```shell
        $ mkdir build
        $ cd build
        $ cmake .. -G "Xcode"
        $ xcodebuild
        $ ./bin/Debug/layout_test
        ```

    - on Linux: use bash script `layout_test/test-linux.sh` or seperate commands

        ```shell
        $ mkdir build
        $ cd build
        $ cmake ..
        $ make
        $ ./bin/layout_test
        ```

6. Please use the correct `Tools/chromedriver` corresponding to your chrome version. If your chrome version > 75, use `goog:loggingPrefs` in `gentest.rb`.
    ```ruby
        caps = Selenium::WebDriver::Remote::Capabilities.chrome(
        "goog:loggingPrefs"=>{
            "browser"=>"ALL",
            "performance"=>"ALL"
        }
        )
    ```
    Otherwise, use `loggingPrefs`.
    ```ruby
        caps = Selenium::WebDriver::Remote::Capabilities.chrome(
        "loggingPrefs"=>{
            "browser"=>"ALL",
            "performance"=>"ALL"
        }
        )
    ```

## TODO List 🚧
- support absolute elements
- optimize LayoutModeAtMost
- add measure function support
