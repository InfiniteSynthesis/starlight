# starlight

[![Build Status](https://travis-ci.com/InfiniteSynthesis/starlight.svg?branch=master)](https://travis-ci.com/InfiniteSynthesis/starlight)

Starlight is a layout engine for cross-platform framework (e.g. React Native)

## features
Support all standard flex properties! Completely the same as [CSS Flexible Box Layout Standards](https://www.w3.org/TR/css-flexbox-1/)

## Testing

For testing we rely on [gtest](https://github.com/google/googletest) as a submodule. For any changes you make you should ensure that all the tests are passing. In case you make any fixes or additions to the library please also add tests for that change to ensure we don't break anything in the future. Tests are located in the `layout_test` directory. You can run test under the following direction.

Instead of manually writing a test which ensures parity with web implementations of Flexbox you can run `layout_test/gentest.rb` to generate a test for you. You can write html which you want to verify in Starlight, in `layout_test/feature` folder, such as the following.

```
<div id="my_test" style="width: 100px; height: 100px; align-items: center;">
  <div style="width: 50px; height: 50px;"></div>
</div>
```

### How to Use

1. install Ruby，cmake

2. install WebDriver

   ```shell
   $ sudo gem install watir
   $ sudo gem install watir-webdriver 
   ```

3. update chromedriver and add to PATH

   ```shell
   $ cd ./layout_test/
   $ ./updatechromedriver.sh
   $ export PATH="$PATH:`pwd`/Tools"
   ```

4. use Ruby to generate test files
    ``` shell
    $ ruby ./gentest.rb
    ```

5. compile and execute on Mac
    use bash script `test.sh` or seperate commands
   ```shell
   $ mkdir build
   $ cd build
   $ cmake .. -G "Xcode"
   $ xcodebuild
   $ ./bin/Debug/layout_test
   ```

6. Please use the correct `Tools/chromedriver` corresponding to your chrome version.
If your chrome version > 75, use `goog:loggingPrefs` in `gentest.rb`
    ```
    caps = Selenium::WebDriver::Remote::Capabilities.chrome(
    "goog:loggingPrefs"=>{
        "browser"=>"ALL",
        "performance"=>"ALL"
    }
    )
    ```
    otherwise, use `loggingPrefs`
    ```
    caps = Selenium::WebDriver::Remote::Capabilities.chrome(
    "loggingPrefs"=>{
        "browser"=>"ALL",
        "performance"=>"ALL"
    }
    )
    ```

## todo list
- support absolute elements
- optimize LayoutModeAtMost
- add measure function support
