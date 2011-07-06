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

class Awesomium : public EventEmitter {
	public:
	static void
	Initialize (v8::Handle<v8::Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> t = FunctionTemplate::New(New);

		t->Inherit(EventEmitter::constructor_template);
		t->InstanceTemplate()->SetInternalFieldCount(1);

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

/*
		t->PrototypeTemplate()->SetAccessor(READY_STATE_SYMBOL, ReadyStateGetter);
		t->PrototypeTemplate()->SetAccessor(MAP_TUPLE_ITEMS_SYMBOL,
																				MapTupleItemsGetter,
																				MapTupleItemsSetter);
*/
		target->Set(String::NewSymbol("Awesomium"), t->GetFunction());
	}

	bool Connect (const char* conninfo)
	{
		if (awesomium_) return false;
		
		/* TODO Ensure that hostaddr is always specified in conninfo to avoid
		 * name lookup. (Unless we're connecting to localhost.)
		 */
		awesomium_ = PQconnectStart(conninfo);
		if (!awesomium_) return false;

		if (PQsetnonblocking(awesomium_, 1) == -1) {
			PQfinish(awesomium_);
			awesomium_ = NULL;
			return false;
		}

		ConnStatusType status = PQstatus(awesomium_);

		if (CONNECTION_BAD == status) {
			PQfinish(awesomium_);
			awesomium_ = NULL;
			return false;
		}

		connecting_ = true;

		int fd = PQsocket(awesomium_);

		ev_io_set(&read_watcher_, fd, EV_READ);
		ev_io_set(&write_watcher_, fd, EV_WRITE);

		/* If you have yet to call PQconnectPoll, i.e., just after the call to
		 * PQconnectStart, behave as if it last returned PGRES_POLLING_WRITING.
		 */
		ev_io_start(EV_DEFAULT_ &write_watcher_);
		
		Ref();

		return true;
	}

	bool Reset ( )
	{
		/* To initiate a awesomium reset, call PQresetStart. If it returns 0,
		 * the reset has failed. If it returns 1, poll the reset using
		 * PQresetPoll in exactly the same way as you would create the
		 * awesomium using PQconnectPoll */
		int r = PQresetStart(awesomium_);
		if (r == 0) return false;

		if (PQsetnonblocking(awesomium_, 1) == -1) {
			PQfinish(awesomium_);
			awesomium_ = NULL;
			return false;
		}

		resetting_ = true;
		ev_io_set(&read_watcher_, PQsocket(awesomium_), EV_READ);
		ev_io_set(&write_watcher_, PQsocket(awesomium_), EV_WRITE);
		ev_io_start(EV_DEFAULT_ &write_watcher_);
		return true;
	}

	bool Query (const char *command) 
	{
		int r = PQsendQuery(awesomium_, command);
		if (r == 0) {
			return false;
		}
		if (PQflush(awesomium_) == 1) ev_io_start(EV_DEFAULT_ &write_watcher_);
		return true;
	}

	void Close (Local<Value> exception = Local<Value>())
	{
		HandleScope scope;
		ev_io_stop(EV_DEFAULT_ &write_watcher_);
		ev_io_stop(EV_DEFAULT_ &read_watcher_);
		PQfinish(awesomium_);
		awesomium_ = NULL;
		if (exception.IsEmpty()) {
			Emit(close_symbol, 0, NULL);
		} else {
			Emit(close_symbol, 1, &exception);
		}
		Unref();
	}

	char * ErrorMessage ( )
	{
		return PQerrorMessage(awesomium_);
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

	static Handle<Value>
	Connect (const Arguments& args)
	{
		Awesomium *awesomium = ObjectWrap::Unwrap<Awesomium>(args.This());

		HandleScope scope;

		if (args.Length() == 0 || !args[0]->IsString()) {
			return ThrowException(Exception::Error(
						String::New("Must give conninfo string as argument")));
		}

		String::Utf8Value conninfo(args[0]->ToString());

		bool r = awesomium->Connect(*conninfo);

		if (!r) {
			return ThrowException(Exception::Error(
						String::New(awesomium->ErrorMessage())));
		}

		return Undefined();
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
		assert(awesomium_ == NULL);
	}

 private:

	void MakeAwesomium ()
	{
		PostgresPollingStatusType status;
		if (connecting_) {
		 status = PQconnectPoll(awesomium_);
		} else {
		 assert(resetting_);
		 status = PQresetPoll(awesomium_);
		}

		if (status == PGRES_POLLING_READING) {
			ev_io_stop(EV_DEFAULT_ &write_watcher_);
			ev_io_start(EV_DEFAULT_ &read_watcher_);
			return;

		} else if (status == PGRES_POLLING_WRITING) {
			ev_io_stop(EV_DEFAULT_ &read_watcher_);
			ev_io_start(EV_DEFAULT_ &write_watcher_);
			return;
		}

		if (status == PGRES_POLLING_OK) {
			Emit(connect_symbol, 0, NULL);
			connecting_ = resetting_ = false;
			ev_io_start(EV_DEFAULT_ &read_watcher_);
			return;
		}

		CloseAwesomiumWithError();
	}

	Local<Value> BuildCell (PGresult *result, int row, int col)
	{
		HandleScope scope;

		if (PQgetisnull(result, row, col)) return scope.Close(Null());

		char *string = PQgetvalue(result, row, col);
		int32_t n = 0, i = 0;

		Oid t = PQftype(result, col);

		Handle<Value> cell;

		switch (t) {
			case BOOLOID:
				cell = *string == 't' ? True() : False();
				break;

			case INT8OID:
			case INT4OID:
			case INT2OID:
				for (i = string[0] == '-' ? 1 : 0; string[i]; i++) {
					n *= 10;
					n += string[i] - '0';
				}
				if (string[0] == '-') n = -n;
				cell = Integer::New(n);
				break;

			default:
#ifndef NDEBUG
//				printf("Unhandled OID: %d\n", t);
#endif
				cell = String::New(string);
		}
		return scope.Close(cell); 
	}

	Local<Value> BuildTuples (PGresult *result)
	{
		HandleScope scope;

		int nrows = PQntuples(result);
		int ncols = PQnfields(result);
		int row_index, col_index;
		char *field_name;

		Local<Array> tuples = Array::New(nrows);

		if (mapTupleItems_) {
			for (row_index = 0; row_index < nrows; row_index++) {
				Local<Object> row = Object::New();
				tuples->Set(Integer::New(row_index), row);

				for (col_index = 0; col_index < ncols; col_index++) {
					field_name = PQfname(result, col_index);
					Local<Value> cell = BuildCell(result, row_index, col_index); 
					row->Set(String::New(field_name), cell);
				}
			}
		} else {
			for (row_index = 0; row_index < nrows; row_index++) {
				Local<Array> row = Array::New(nrows);
				tuples->Set(Integer::New(row_index), row);

				for (col_index = 0; col_index < ncols; col_index++) {
					Local<Value> cell = BuildCell(result, row_index, col_index); 
					row->Set(Integer::New(col_index), cell);
				}
			}
		}

		return scope.Close(tuples);
	}

	void CloseAwesomiumWithError (const char *message_s = NULL)
	{
		HandleScope scope;

		if (!message_s) message_s = PQerrorMessage(awesomium_);
		Local<String> message = String::New(message_s);
		Local<Value> exception = Exception::Error(message);

		Close(exception);
	}

	static inline
	Local<Value> BuildResultException (PGresult *result)
	{
		HandleScope scope;

		char *primary_s = PQresultErrorField(result, PG_DIAG_MESSAGE_PRIMARY);
		assert(primary_s);
		Local<String> primary = String::New(primary_s);

		Local<Value> error_v = Exception::Error(primary);
		assert(error_v->IsObject());
		Local<Object> error = Local<Object>::Cast(error_v);

		char *full_s = PQresultErrorMessage(result);
		if (full_s) {
			error->Set(String::NewSymbol("full"), String::New(full_s));
		}

		char *detail_s = PQresultErrorField(result, PG_DIAG_MESSAGE_DETAIL);
		if (detail_s) {
			error->Set(String::NewSymbol("detail"), String::New(detail_s));
		}

		char *severity_s = PQresultErrorField(result, PG_DIAG_SEVERITY);
		if (severity_s) {
			error->Set(String::NewSymbol("severity"), String::New(severity_s));
		}

		/* TODO PG_DIAG_SQLSTATE
		 *			PG_DIAG_MESSAGE_HINT
		 *			PG_DIAG_STATEMENT_POSITION
		 *			PG_DIAG_INTERNAL_POSITION
		 *			PG_DIAG_INTERNAL_QUERY
		 *			PG_DIAG_CONTEXT
		 *			PG_DIAG_SOURCE_FILE
		 *			PG_DIAG_SOURCE_LINE
		 *			PG_DIAG_SOURCE_FUNCTION
		 */

		return scope.Close(error);
	}

	void EmitResult (PGresult *result)
	{
		HandleScope scope;

		Local<Value> tuples;
		Local<Value> exception;

		Local<Value> args[2];

		switch (PQresultStatus(result)) {
			case PGRES_EMPTY_QUERY:
			case PGRES_COMMAND_OK:
				Emit(result_symbol, 0, NULL);
				break;

			case PGRES_TUPLES_OK:
				args[0] = Local<Value>::New(Null());
				args[1] = BuildTuples(result);
				Emit(result_symbol, 2, args);
				break;

			case PGRES_COPY_OUT:
			case PGRES_COPY_IN:
				assert(0 && "Not yet implemented.");
				exception = Exception::Error(String::New("Not yet implemented"));
				Emit(result_symbol, 1, &exception);
				break;

			case PGRES_BAD_RESPONSE:
			case PGRES_NONFATAL_ERROR:
			case PGRES_FATAL_ERROR:
				exception = BuildResultException(result);
				Emit(result_symbol, 1, &exception);
				break;
		}
	}

	void EmitNotify (PGnotify *notify)
	{
		HandleScope scope;

		Local<Value> args[3];

		args[0] = Local<Value>::New(String::New(notify->relname));
		args[1] = Local<Value>::New(Integer::New(notify->be_pid));
		args[2] = Local<Value>::New(String::New(notify->extra));
		Emit(notify_symbol, 3, args);
	}

	void Event (int revents)
	{
		if (revents & EV_ERROR) {
			CloseAwesomiumWithError("awesomium closed");
			return;
		}

		assert(PQisnonblocking(awesomium_));

		if (connecting_ || resetting_) {
			MakeAwesomium();
			return;
		}

		if (revents & EV_READ) {
			if (PQconsumeInput(awesomium_) == 0) {
				CloseAwesomiumWithError();
				return;
			}

			if (!PQisBusy(awesomium_)) {
				PGresult *result;
				while ((result = PQgetResult(awesomium_))) {
					EmitResult(result);
					PQclear(result);
				}
				Emit(ready_symbol, 0, NULL);
			}

			PGnotify *notify;
			while ((notify = PQnotifies(awesomium_))) {
				EmitNotify(notify);
				PQfreemem(notify);
			}
		}

		if (revents & EV_WRITE) {
			if (PQflush(awesomium_) == 0) ev_io_stop(EV_DEFAULT_ &write_watcher_);
		}
	}

	static void
	io_event (EV_P_ ev_io *w, int revents)
	{
		Awesomium *awesomium = static_cast<Awesomium*>(w->data);
		awesomium->Event(revents);
	}

	ev_io read_watcher_;
	ev_io write_watcher_;
	PGconn *awesomium_;
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
