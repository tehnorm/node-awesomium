// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview This implementes a future promise class.
 */

cr.define('cr', function() {

  /**
   * Sentinel used to mark a value as pending.
   */
  const PENDING_VALUE = {};

  /**
   * Creates a future promise.
   * @param {Function=} opt_callback Callback.
   * @constructor
   */
  function Promise(opt_callback) {
    /**
     * An array of the callbacks.
     * @type {!Array.<!Function>}
     * @private
     */
    this.callbacks_ = opt_callback ? [opt_callback] : [];
  }

  Promise.prototype = {
    /**
     * The current value.
     * @type {*}
     * @private
     */
    value_: PENDING_VALUE,

    /**
     * The value of the future promise. Accessing this before the promise has
     * been fulfilled will throw an error. If this is set to an exception
     * accessing this will throw as well.
     * @type {*}
     */
    get value() {
      return this.done ? this.value_ : undefined;
    },
    set value(value) {
      if (!this.done) {
        this.value_ = value;
        for (var i = 0; i < this.callbacks_.length; i++) {
          this.callbacks_[i].call(null, value);
        }
        this.callbacks_.length = 0;
      }
    },

    /**
     * Whether the future promise has been fulfilled.
     * @type {boolean}
     */
    get done() {
      return this.value_ !== PENDING_VALUE;
    },

    /**
     * Adds a listener to the future promise. The function will be called when
     * the promise is fulfilled. If the promise is already fullfilled this will
     * never call the function.
     * @param {!Function} fun The function to call.
     */
    addListener: function(fun) {
      if (this.done)
        fun(this.value);
      else
        this.callbacks_.push(fun);
    },

    /**
     * Removes a previously added listener from the future promise.
     * @param {!Function} fun The function to remove.
     */
    removeListener: function(fun) {
      var i = this.callbacks_.indexOf(fun);
      if (i >= 0)
        this.callbacks_.splice(i, 1);
    },

    /**
     * If the promise is done then this returns the string representation of
     * the value.
     * @return {string} The string representation of the promise.
     * @override
     */
    toString: function() {
      if (this.done)
        return String(this.value);
      else
        return '[object Promise]';
    },

    /**
     * Override to allow arithmetic.
     * @override
     */
    valueOf: function() {
      return this.value;
    }
  };

  /**
   * When a future promise is done call {@code fun}. This also calls the
   * function if the promise has already been fulfilled.
   * @param {!Promise} p The promise.
   * @param {!Function} fun The function to call when the promise is fulfilled.
   */
  Promise.when = function(p, fun) {
    p.addListener(fun);
  };

  /**
   * Creates a new promise the will be fulfilled after {@code t} ms.
   * @param {number} t The time to wait before the promise is fulfilled.
   * @param {*=} opt_value The value to return after the wait.
   * @return {!Promise} The new future promise.
   */
  Promise.wait = function(t, opt_value) {
    var p = new Promise;
    window.setTimeout(function() {
      p.value = opt_value;
    }, t);
    return p;
  };

  /**
   * Creates a new future promise that is fulfilled when any of the promises are
   * fulfilled.
   * @param {...!Promise} var_args The promises used to build up the new
   *     promise.
   * @return {!Promise} The new promise that will be fulfilled when any of th
   *     passed in promises are fulfilled.
   */
  Promise.any = function(var_args) {
    var p = new Promise;
    function f(v) {
      p.value = v;
    }
    for (var i = 0; i < arguments.length; i++) {
      arguments[i].addListener(f);
    }
    return p;
  };

  /**
   * Wraps an event in a future promise.
   * @param {!EventTarget} target The object that dispatches the event.
   * @param {string} type The type of the event.
   * @param {boolean=} opt_useCapture Whether to listen to the capture phase or
   *     the bubble phase.
   * @return {!Promise} The promise that will be fulfilled when the event is
   *     dispatched.
   */
  Promise.event = function(target, type, opt_useCapture) {
    var p = new Promise;
    target.addEventListener(type, function(e) {
      p.value = e;
    }, opt_useCapture);
    return p;
  };

  return {
    Promise: Promise
  };
});
