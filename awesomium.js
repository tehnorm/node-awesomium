var FFI = require("../node-ffi");

var AwesomiumWebCore = new FFI.Library("./Awesomium");


var view = AwesomiumWebCore.createWebView(500, 500);
