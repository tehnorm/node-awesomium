var A = require("./awesomium");
var util = require('util');

util.debug(A);
var browser = A.Awesomium.createBrowser(960, 1280);
util.debug(browser);
//browser.createWebView(960, 1280);
//browser.addEventHandler('', function(e){

//});


//browser.loadURL('google.com');
