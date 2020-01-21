#!/usr/bin/env ruby

require 'watir'
require 'fileutils'

caps = Selenium::WebDriver::Remote::Capabilities.chrome(
  "goog:loggingPrefs"=>{
    "browser"=>"ALL",
    "performance"=>"ALL"
  }
)
browser = Watir::Browser.new(:chrome, :desired_capabilities => caps, :switches => ['--force-device-scale-factor=1', '--window-position=0,0'])

Dir.chdir(File.dirname($0))

if File.directory?('./destination') != true
  Dir.mkdir('./destination')
end

if File.directory?('./cxx') != true
  Dir.mkdir('./cxx')
end

names = ""

Dir['feature/*html'].each do | file |
  fixture = File.read(file)
  name = File.basename(file, '.*')
  template = File.open('template.html').read
  f = File.open("./destination/#{name}.html", 'w')

  f.write sprintf(template, fixture)
  f.close

  
  browser.goto('file://' + Dir.pwd + "/destination/#{name}.html")
  logs = browser.driver.manage.logs.get(:browser)

  f = File.open("./cxx/#{name}.hpp", 'w')
  f.write eval(logs[0].message.sub(/^[^"]*/, '')).sub('$LayoutTest', name).sub('$LayoutTest', name).sub('$LayoutTest', name)
  f.close
  names += "#include<" + "#{name}" + ".hpp>\n"
end
browser.close
                                   
template = File.open('./src/main.template').read
f = File.open("./src/main.cpp", 'w')

f.write sprintf(template, names)
f.close
                                   

