var bindings = require("./build/default/nodeium");

var Awesomium = bindings.Nodeium;

exports.createBrowser = function (h, w){
	var a = new Awesomium();
	
	a.createBrowser(h, w);

	return a;
};
