# Layout Test

Use test case to verify the correctness of layout results.

### How to Use

1. install Ruby，cmake

2. install WebDriver

   ```shell
   $ sudo gem install watir
   $ sudo gem install watir-webdriver 
   ```

3. add layout_test/Tools/ to PATH

   ```shell
   $ cd lynx-native/layout_test/
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