var A = require("./build/default/awesomium");


var browser = A.createBrowser();
browser.createWebView(960, 1280);
browser.addEventHandler('', function(e){

});


browser.loadURL('google.com');
