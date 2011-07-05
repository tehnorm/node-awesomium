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
