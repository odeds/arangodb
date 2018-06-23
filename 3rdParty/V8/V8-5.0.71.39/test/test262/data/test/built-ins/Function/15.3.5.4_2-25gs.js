// Copyright (c) 2012 Ecma International.  All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/*---
es5id: 15.3.5.4_2-25gs
description: >
    Strict mode - checking access to strict function caller from
    strict function (FunctionExpression defined within a
    FunctionExpression inside strict mode)
flags: [onlyStrict]
---*/

var f1 = function () {
    var f = function () {
        gNonStrict();
    }
    f();
}

assert.throws(TypeError, function() {
    f1();
});

function gNonStrict() {
    return gNonStrict.caller;
}