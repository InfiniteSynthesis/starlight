set -eu

if [ "${TRAVIS_OS_NAME}" != "osx" ]; then
    echo "Not a macOS build; skipping installation"
    exit 0
fi

brew update
brew install cmake

sudo gem install watir
sudo gem install watir-webdriver 