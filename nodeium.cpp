#include <Awesomium/WebCore.h>
#include <v8.h>

using namespace v8;

// all node plugins must emit
// a "init" function
extern "C" void
init (Handle<Object> target) {
    HandleScope scope;
    Local<String> output = String::New("hello javascript!");
    target->Set(String::New("greeting"), output);
} 
