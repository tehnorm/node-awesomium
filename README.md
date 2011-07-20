# Node Bindings for Awesomium

This is a simple API that exposes portions of the [Amwesomium](http://awesomium.com) library. In short it brings real webkit to node. Can you say browser from the command line that can do real browser things!?

Currently only portions of the library are exposed on the account of me knowing dang close to zero C++. So, be nice if you have feedback. Also would love some more experienced C++ hands if you have the gumption to contribute.

## The API

* Browser = Nodeium.createBrowser(height, width)
* Browser.loadUrl('http://www.uri.tld')
* Browser.addListener(type, callback)
* Browser.toJPEG('/path/and/file/name.jpeg')
* Browser.excuteJavascriptWithResult(script)
