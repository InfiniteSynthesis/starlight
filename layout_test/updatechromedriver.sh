CHROME_VERSION=$(/Applications/Google\ Chrome.app/Contents/MacOS/Google\ Chrome --version)
CHROME_VERSION_V=${CHROME_VERSION:14:2}

DRIVER_VERSION=$(curl -s wget https://chromedriver.storage.googleapis.com/LATEST_RELEASE_$CHROME_VERSION_V)

wget https://chromedriver.storage.googleapis.com/$DRIVER_VERSION/chromedriver_mac64.zip

unzip chromedriver_mac64.zip

mv ./chromedriver ./Tools/

chmod +x ./Tools/chromedriver