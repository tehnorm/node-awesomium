#include <v8.h>
#include <node.h>
#include <iostream>
#include <node_events.h>
#include <assert.h>
#include <stdlib.h>
#include <Awesomium/WebCore.h>
#include <string>
#include <sstream>
#include <unistd.h>
 
using namespace node;
using namespace v8;
using namespace std;

/* This code is PUBLIC DOMAIN, and is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND. See the accompanying 
 * LICENSE file.
 */

#include <v8.h>
#include <node.h>

#include <unistd.h>

using namespace node;
using namespace v8;

#define REQ_FUN_ARG(I, VAR)                                             \
  if (args.Length() <= (I) || !args[I]->IsFunction())                   \
    return ThrowException(Exception::TypeError(                         \
                  String::New("Argument " #I " must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

class Nodeium: ObjectWrap
{
private:
  int m_count;
public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("Nodeium"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "hello", Hello);

    target->Set(String::NewSymbol("Nodeium"),
                s_ct->GetFunction());
  }

  Nodeium() :
    m_count(0)
  {
  }

  ~Nodeium()
  {
  }

  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    Nodeium* hw = new Nodeium();
    hw->Wrap(args.This());
    return args.This();
  }

  struct hello_baton_t {
    Nodeium *hw;
    int increment_by;
    int sleep_for;
    Persistent<Function> cb;
  };

  static Handle<Value> Hello(const Arguments& args)
  {
    HandleScope scope;

    REQ_FUN_ARG(0, cb);

    Nodeium* hw = ObjectWrap::Unwrap<Nodeium>(args.This());

    hello_baton_t *baton = new hello_baton_t();
    baton->hw = hw;
    baton->increment_by = 2;
    baton->sleep_for = 1;
    baton->cb = Persistent<Function>::New(cb);

    hw->Ref();

    eio_custom(EIO_Hello, EIO_PRI_DEFAULT, EIO_AfterHello, baton);
    ev_ref(EV_DEFAULT_UC);

    return Undefined();
  }


  static int EIO_Hello(eio_req *req)
  {
    hello_baton_t *baton = static_cast<hello_baton_t *>(req->data);

    sleep(baton->sleep_for);

    baton->hw->m_count += baton->increment_by;

    return 0;
  }

  static int EIO_AfterHello(eio_req *req)
  {
    HandleScope scope;
    hello_baton_t *baton = static_cast<hello_baton_t *>(req->data);
    ev_unref(EV_DEFAULT_UC);
    baton->hw->Unref();

    Local<Value> argv[1];

    argv[0] = String::New("Hello World");

    TryCatch try_catch;

    baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

    baton->cb.Dispose();

    delete baton;
    return 0;
  }

};

Persistent<FunctionTemplate> Nodeium::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    Nodeium::Init(target);
  }

  NODE_MODULE(nodeium, init);
}

