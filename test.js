var A = require("./build/default/awesomium");
var util = require('util');


var browser = A.createBrowser(960, 1280);
util.debug(browser);
//browser.createWebView(960, 1280);
//browser.addEventHandler('', function(e){

//});


//browser.loadURL('google.com');
