/**
 * This is a simple "Hello World!" example of using Awesomium.
 *
 * It loads a page, renders it once, and saves it to a file.
 *
 * Procedure:
 * -- Create the WebCore singleton
 * -- Create a new WebView and request for it to load a URL.
 * -- Wait for the WebView to finish loading.
 * -- Render the loaded page to a buffer.
 * -- Save the buffer to 'result.jpg'.
 * -- Clean up.
 */
#include <v8.h>
#include <node.h>
#include <iostream>
 
// Various included headers
#include <Awesomium/WebCore.h>
#include <iostream>
#include <string>
#include <sstream>
#if defined(__WIN32__) || defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <unistd.h>
#endif

using namespace node;
using namespace v8;
using namespace std;


#define REQ_FUN_ARG(I, VAR)                                             \
  if (args.Length() <= (I) || !args[I]->IsFunction())                   \
    return ThrowException(Exception::TypeError(                         \
                  String::New("Argument " #I " must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

class Awesomium: ObjectWrap
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
    s_ct->SetClassName(String::NewSymbol("Awesomium"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "hello", Hello);

    target->Set(String::NewSymbol("Awesomium"),
                s_ct->GetFunction());
  }

  Awesomium() :
    m_count(0)
  {
  }

  ~Awesomium()
  {
  }

  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    Awesomium* hw = new Awesomium();
    hw->Wrap(args.This());
    return args.This();
  }

  struct hello_baton_t {
    Awesomium *hw;
    int increment_by;
    int sleep_for;
    Persistent<Function> cb;
  };

  static Handle<Value> Hello(const Arguments& args)
  {
    HandleScope scope;

    REQ_FUN_ARG(0, cb);

    Awesomium* hw = ObjectWrap::Unwrap<Awesomium>(args.This());

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

Persistent<FunctionTemplate> Awesomium::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    Awesomium::Init(target);
  }

  NODE_MODULE(awesomium, init);
}



/* OLD CODE 

// Various macro definitions
#define WIDTH	1024
#define HEIGHT	768
#define URL	"http://app.argylesocial.com"
#define SLEEP_MS	50

int rCount;

std::string i2string(int i) {
	std::ostringstream buffer;
	buffer << i;
	return buffer.str();
}

int	wash (Awesomium::WebView* view, Awesomium::WebCore* core){
	if(view->isDirty()){
//		while(view->isDirty() == 1){
			std::cout << view->isDirty() << std::endl;
			usleep(SLEEP_MS * 1000);
			core->update();
//		}
	}else{
			std::cout << "Clean!" << std::endl;
	}
	return 0;
}

void render (Awesomium::WebView* view, Awesomium::WebCore* core){
	//std::string name ;
	//name = "test" + i2string(rCount) + ".jpg";
	
	// Get our rendered buffer from our WebView. All actual rendering takes 
	// place in our WebView sub-process which passes the rendered data to our 
	// main process during each call to WebCore::update.
	const Awesomium::RenderBuffer* renderBuffer = view->render();
	wash(view, core);	
	
	// Make sure our render buffer is not NULL-- WebView::render will return
	// NULL if the WebView process has crashed.
	if(renderBuffer != NULL)
	{
		
		
		// Save our RenderBuffer directly to a JPEG image
		std::string fname;
		//fname = "./" + name;
		//  char buffer [50];
//		long lb [50];
		//fname = "./test%s.jpg";
		//sprintf(buffer, "test%s.jpg", "1");
		
		std::stringstream sstr;
		sstr << "./test" << rCount << ".jpg";
		fname = sstr.str();
		
		// sprintf(lb, "%s", buffer);
		
		std::cout << fname << std::endl;
		
		std::wstring fname2(fname.length(), L' ');
		std::copy(fname.begin(), fname.end(), fname2.begin());
		
		std::cout << "Saved a render of the page to '" + fname + "'." << std::endl;
		
		//renderBuffer->saveToJPEG(L"test.jpg");
		renderBuffer->saveToJPEG(fname2);
		
		rCount = rCount + 1;
		
		
		// Open up the saved JPEG
#if defined(__WIN32__) || defined(_WIN32)
		system("start " + fname2);
#elif defined(__APPLE__)
		const char *c = ("open " + fname).c_str();
		system(c);
#endif
	}else {
		std::cout << "No buffer." << std::endl;
	}	
}

// Our main program
int main()
{
	// Create our WebCore singleton with the default options
	Awesomium::WebCore* webCore = new Awesomium::WebCore();
	
	// Create a new WebView instance with a certain width and height, using the 
	// WebCore we just created
	Awesomium::WebView* webView = webCore->createWebView(WIDTH, HEIGHT);
	
	// Load a certain URL into our WebView instance
	webView->loadURL(URL);
	
	std::cout << "Page is now loading..." << std::endl;;
	
	// Wait for our WebView to finish loading
	while(webView->isLoadingPage())
	{
		// Sleep a little bit so we don't consume too much CPU while waiting 
		// for the page to finish loading.
#if defined(__WIN32__) || defined(_WIN32)
		Sleep(SLEEP_MS);
#elif defined(__APPLE__)
		usleep(SLEEP_MS * 1000);
#endif

		// We must call WebCore::update in our update loop.
		webCore->update();
	}

	std::cout << "Page has finished loading." << std::endl;

//	webView->executeJavascript("document.body.email.value = 'test';");
	webView->executeJavascript("jQuery('#email').val('testtest');");
	webView->executeJavascript("jQuery('#password').val('testtest');");

	wash(webView, webCore);
	render(webView, webCore);
	std::cout << "loading: " << webView->isLoadingPage() << std::endl;
//	string test;
//	test = webView->executeJavascriptWithResult("alert('testing')").get().toString();
	int width = webView->executeJavascriptWithResult("document.body.scrollWidth").get().toInteger();
	webView->executeJavascript("jQuery('form').trigger('submit');");
	wash(webView, webCore);
	
	std::cout << width << std::endl;
	render(webView, webCore);
	
	sleep(5);
	std::cout << "loading: " << webView->isLoadingPage() << std::endl;
	wash(webView, webCore);
	render(webView, webCore);
	sleep(5);
	// Destroy our WebView instance
	webView->destroy();
	
	// Destroy our WebCore instance
	delete webCore;
	
	return 0;
}

*/
