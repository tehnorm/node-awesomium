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

static Persistent<String> ready_symbol;
static Persistent<String> result_symbol;
static Persistent<String> close_symbol;
static Persistent<String> connect_symbol;
static Persistent<String> notify_symbol;
// #define READY_STATE_SYMBOL String::NewSymbol("readyState")
// #define MAP_TUPLE_ITEMS_SYMBOL String::NewSymbol("mapTupleItems")

	/* EVENTS the Awesomium::WebViewListener Exposes
		onBeginNavigation
		onBeginLoading
		onFinishLoading
		onCallback
		onReceiveTitle
		onChangeTooltip
		onChangeCursor
		onChangeKeyboardFocus
		onChangeTargetURL
		onOpenExternalLink
		onRequestDownload
		onWebViewCrashed
		onPluginCrashed
		onRequestMove
		onGetPageContents
		onDOMReady
		onRequestFileChooser
		onGetScrollData
		onJavascriptConsoleMessage
	*/

class Nodeium : public EventEmitter {


	public:
	static void
	Initialize (v8::Handle<v8::Object> target)
	{
		HandleScope scope;
		Local<FunctionTemplate> t = FunctionTemplate::New(New);

		t->Inherit(EventEmitter::constructor_template);
		t->InstanceTemplate()->SetInternalFieldCount(1);
/*
		close_symbol = NODE_PSYMBOL("close");
		connect_symbol = NODE_PSYMBOL("connect");
		result_symbol = NODE_PSYMBOL("result");
		ready_symbol = NODE_PSYMBOL("ready");
		notify_symbol = NODE_PSYMBOL("notify");

		NODE_SET_PROTOTYPE_METHOD(t, "connect", Connect);
		NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
		NODE_SET_PROTOTYPE_METHOD(t, "reset", Reset);
		NODE_SET_PROTOTYPE_METHOD(t, "dispatchQuery", DispatchQuery);
		NODE_SET_PROTOTYPE_METHOD(t, "escapeString", EscapeString);

		t->PrototypeTemplate()->SetAccessor(READY_STATE_SYMBOL, ReadyStateGetter);
		t->PrototypeTemplate()->SetAccessor(MAP_TUPLE_ITEMS_SYMBOL,
						MapTupleItemsGetter,
						MapTupleItemsSetter);
*/
		target->Set(String::NewSymbol("Nodeium"), t->GetFunction());

	}

	static Handle<Value> createBrowser(int height, int width)
	{
		// Create our WebCore singleton with the default options
		Awesomium::WebCore* webCore = new Awesomium::WebCore();
		
		// Create a new WebView instance with a certain width and height, using the 
		// WebCore we just created
		Awesomium::WebView* webView = webCore->createWebView(height, width);
		return Undefined();
	}


	protected:

	Nodeium () : EventEmitter ()
	{
		webView = NULL;

	//	connecting_ = resetting_ = false;

		ev_init(&read_watcher_, io_event);
		read_watcher_.data = this;

		ev_init(&write_watcher_, io_event);
		write_watcher_.data = this;
	}

	~Nodeium ()
	{
		assert(webView == NULL);
	}

	static Handle<Value>
	New (const Arguments& args)
	{
		HandleScope scope;

		Nodeium *browser = new Nodeium();
		browser->Wrap(args.This());

		return args.This();
	}

	private:

	static void
	io_event (EV_P_ ev_io *w, int revents)
	{
		Nodeium *webview = static_cast<Nodeium*>(w->data);
//		connection->Event(revents);
	}

	ev_io read_watcher_;
	ev_io write_watcher_;


	Awesomium::WebCore *webCore;
	Awesomium::WebView *webView;
};
 
extern "C" {
  static void init(Handle<Object> target)
  {
	HandleScope scope;
	Nodeium::Initialize(target);
//	NODE_SET_METHOD(target, "createBrowser", createBrowser);
  }
 
  NODE_MODULE(Nodeium, init);
}

/*
class Awesomium : public EventEmitter {
	public:
	static void
	Initialize (v8::Handle<v8::Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> t = FunctionTemplate::New(New);

		t->Inherit(EventEmitter::constructor_template);
		t->InstanceTemplate()->SetInternalFieldCount(1);


		close_symbol = NODE_PSYMBOL("close");
		connect_symbol = NODE_PSYMBOL("connect");
		result_symbol = NODE_PSYMBOL("result");
		ready_symbol = NODE_PSYMBOL("ready");
		notify_symbol = NODE_PSYMBOL("notify");

		NODE_SET_METHOD(t, "createBrowser", createBrowser);
		NODE_SET_PROTOTYPE_METHOD(t, "connect", Connect);
		NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
		NODE_SET_PROTOTYPE_METHOD(t, "reset", Reset);
		NODE_SET_PROTOTYPE_METHOD(t, "dispatchQuery", DispatchQuery);
		NODE_SET_PROTOTYPE_METHOD(t, "escapeString", EscapeString);
		t->PrototypeTemplate()->SetAccessor(READY_STATE_SYMBOL, ReadyStateGetter);
		t->PrototypeTemplate()->SetAccessor(MAP_TUPLE_ITEMS_SYMBOL,
																				MapTupleItemsGetter,
																				MapTupleItemsSetter);
		target->Set(String::NewSymbol("Awesomium"), t->GetFunction());
	}


	static Handle<Value> createBrowser(const Arguments& args)
	{

	}



 protected:

	static Handle<Value>
	New (const Arguments& args)
	{
		HandleScope scope;

		Awesomium *awesomium = new Awesomium();
		awesomium->Wrap(args.This());

		return args.This();
	}

	Awesomium () : EventEmitter () 
	{
		awesomium_ = NULL;

		connecting_ = resetting_ = false;

		ev_init(&read_watcher_, io_event);
		read_watcher_.data = this;

		ev_init(&write_watcher_, io_event);
		write_watcher_.data = this;
	}

	~Awesomium ()
	{
//		assert(awesomium_ == NULL);
	}

 private:

	void MakeAwesomium ()
	{
	}


	ev_io read_watcher_;
	ev_io write_watcher_;
	bool connecting_;
	bool resetting_;
	bool mapTupleItems_;
};

extern "C" void
init (Handle<Object> target) 
{
	HandleScope scope;
	Awesomium::Initialize(target);
}

*/


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
		//	char buffer [50];
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
