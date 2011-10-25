var
  util = require('util'),
  nodium = require('../nodium');

console.log(nodium.hello());
console.log(util.inspect(nodium, true, null));
