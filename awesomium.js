var nodeium = require("./build/default/nodeium");

var Awesomium = nodeium.Nodeium;

exports.createBrowser = function (h, w){
	var a = new Awesomium();
	
	a.createBrowser(h, w);

	return a;
};
