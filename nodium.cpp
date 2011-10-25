// Headers for v8/Node
#include <v8.h>
#include <node.h>

// Headers for Awesomium
#include <Awesomium/WebCore.h>
#include <iostream>

#if defined(__WIN32__) || defined(_WIN32)
  #include <windows.h>
  #define sleep(x) Sleep(x)
#elif defined(__APPLE__) || defined(__linux__)
  #include <unistd.h>
  #define sleep(x) usleep(x * 1000)
#endif

// Various macro definitions
#define WIDTH 512
#define HEIGHT 512
#define URL "http://www.google.com"
#define SLEEP_MS 50

using namespace node;
using namespace v8;

// hello world program for awesomium as node module
static int hw()
{
	// create webcore singleton with the default options
	Awesomium::WebCore* webCore = new Awesomium::WebCore();
	
	// create webview singleton at width x height resolution
	Awesomium::WebView* webView = webCore->createWebView(WIDTH, HEIGHT);
	
	webView->loadURL(URL);
	
	std::cout << "Page loading..." << std::endl;;
	
	// wait for webview to finish loading
	while(webView->isLoadingPage())
	{
		sleep(SLEEP_MS);

		// required
		webCore->update();
	}

	std::cout << "Page loaded." << std::endl;
	
	const Awesomium::RenderBuffer* renderBuffer = webView->render();
	
	if(renderBuffer != NULL)
	{
		renderBuffer->saveToJPEG(L"./page.jpg");
		
		std::cout << "Saved page render as page.jpg" << std::endl;
	}
	
	// destroy webview instance
	webView->destroy();
	
	// destroy webcore instance
	delete webCore;
	
	return 0;
}

static Handle<Value> hello(const Arguments& args)
{
	hw();
	return String::New("Hello World");
}

extern "C" {
static void init(Handle<Object> target)
{
	NODE_SET_METHOD(target, "hello", hello);
}

	NODE_MODULE(nodium, init);
}

