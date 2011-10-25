/**
 * @file WebViewListener.h
 *
 * @author
 *
 * This file is a part of Awesomium, a library that makes it easy for 
 * developers to embed the web in their applications.
 *
 * Website: <http://www.awesomium.com>
 * 
 * Copyright (C) 2011 Khrona. All rights reserved. Awesomium is a 
 * trademark of Khrona.
 */

#ifndef __WEBVIEWLISTENER_H__
#define __WEBVIEWLISTENER_H__

#include <string>
#include <Awesomium/JSValue.h>
#include <Awesomium/RenderBuffer.h>

#if defined(_WIN32)
#include <windows.h>
#endif

namespace Awesomium {

	/**
	* An enumeration of all the possible web cursors. Used with 
	* WebViewListener::onChangeCursor.
	*/
	enum CursorType 
	{
        CursorPointer,
        CursorCross,
        CursorHand,
        CursorIBeam,
        CursorWait,
        CursorHelp,
        CursorEastResize,
        CursorNorthResize,
        CursorNorthEastResize,
        CursorNorthWestResize,
        CursorSouthResize,
        CursorSouthEastResize,
        CursorSouthWestResize,
        CursorWestResize,
        CursorNorthSouthResize,
        CursorEastWestResize,
        CursorNorthEastSouthWestResize,
        CursorNorthWestSouthEastResize,
        CursorColumnResize,
        CursorRowResize,
        CursorMiddlePanning,
        CursorEastPanning,
        CursorNorthPanning,
        CursorNorthEastPanning,
        CursorNorthWestPanning,
        CursorSouthPanning,
        CursorSouthEastPanning,
        CursorSouthWestPanning,
        CursorWestPanning,
        CursorMove,
        CursorVerticalText,
        CursorCell,
        CursorContextMenu,
        CursorAlias,
        CursorProgress,
        CursorNoDrop,
        CursorCopy,
        CursorNone,
        CursorNotAllowed,
        CursorZoomIn,
        CursorZoomOut,
        CursorCustom
	};

	enum IMEState
	{
		IME_DISABLE = 0,
		IME_MOVE_WINDOW = 1,
		IME_COMPLETE_COMPOSITION = 2
	};
	
	class WebView;
	
	/**
	 * WebViewListener is a virtual interface that you can use to receive 
	 * notifications from a certain WebView. Simply make a class that inherits
	 * from WebViewListener and register it via WebView::setListener.
	 */
	class _OSMExport WebViewListener
	{
	public:
		/**
		 * This event is fired when a WebView begins navigating to a new URL.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	url		The URL that is being navigated to.
		 *
		 * @param	frameName	The name of the frame that this event 
		 *                      originated from.
		 */
		virtual void onBeginNavigation(Awesomium::WebView* caller, 
									   const std::string& url, 
									   const std::wstring& frameName) = 0;
		
		/**
		 * This event is fired when a WebView begins to actually receive data 
		 * from a server.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	url		The URL of the frame that is being loaded.
		 *
		 * @param	frameName	The name of the frame that this event 
		 *                      originated from.
		 *
		 * @param	statusCode	The HTTP status code returned by the server.
		 *
		 * @param	mimeType	The mime-type of the content that is being 
		 *                      loaded.
		 */
		virtual void onBeginLoading(Awesomium::WebView* caller, 
									const std::string& url, 
									const std::wstring& frameName, 
									int statusCode, 
									const std::wstring& mimeType) = 0;
		
		/**
		 * This event is fired when all loads have finished for a WebView.
		 *
		 * @param	caller	The WebView that fired the event.
		 */
		virtual void onFinishLoading(Awesomium::WebView* caller) = 0;
		
		/**
		 * This event is fired when a Client callback has been invoked via 
		 * Javascript from a page.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	objectName	The name of the Javascript Object that contains 
		 *                      the invoked callback.
		 *
		 * @param	callbackName	The name of the callback that was invoked 
		 *                          (must have been previously bound via 
		 *                          WebView::setObjectCallback).
		 *
		 * @param	args	The arguments passed to the callback.
		 */
		virtual void onCallback(Awesomium::WebView* caller, 
								const std::wstring& objectName, 
								const std::wstring& callbackName, 
								const Awesomium::JSArguments& args) = 0;
		
		/**
		 * This event is fired when a page title is received.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	title	The page title.
		 *
		 * @param	frameName	The name of the frame that this event 
		 *                      originated from.
		 */
		virtual void onReceiveTitle(Awesomium::WebView* caller, 
									const std::wstring& title, 
									const std::wstring& frameName) = 0;
		
		/**
		 * This event is fired when a tooltip has changed state.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	tooltip		The tooltip text (or, is an empty string when 
		 *                      the tooltip should disappear).
		 */
		virtual void onChangeTooltip(Awesomium::WebView* caller, 
									 const std::wstring& tooltip) = 0;
		
		/**
		 * This event is fired when the cursor has changed state.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	cursor	The cursor type.
		 */
		virtual void onChangeCursor(Awesomium::WebView* caller, 
									Awesomium::CursorType cursor) = 0;
		
		/**
		 * This event is fired when keyboard focus has changed.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	isFocused	Whether or not the keyboard is currently 
		 *                      focused.
		 */
		virtual void onChangeKeyboardFocus(Awesomium::WebView* caller, 
										   bool isFocused) = 0;
		
		/**
		 * This event is fired when the target URL has changed. This is usually 
		 * the result of hovering over a link on the page.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	url	The updated target URL (or empty if the target URL is 
		 *              cleared).
		 */
		virtual void onChangeTargetURL(Awesomium::WebView* caller, 
									   const std::string& url) = 0;
		
		/**
		 * This event is fired when an external link is attempted to be opened.
		 * An external link is any link that normally opens in a new window in 
		 * a standard browser (for example, links with target="_blank", calls 
		 * to window.open(url), and URL open events from Flash plugins).
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	url		The URL of the external link.
		 *
		 * @param	source	The URL of the page that initiated the event. 
		 *                  May be blank.
		 */
		virtual void onOpenExternalLink(Awesomium::WebView* caller, 
										const std::string& url, 
										const std::wstring& source) = 0;

		/**
		 * This event is fired when the page requests a certain URL to be
		 * downloaded by the user.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	url		The URL of the file requested to be downloaded.
		 */
		virtual void onRequestDownload(Awesomium::WebView* caller,
										const std::string& url) = 0;
		
		/**
		 * This event is fired when the renderer for a certain WebView (which is 
		 * isolated in a separate process) crashes unexpectedly.
		 *
		 * @param	caller	The WebView that fired the event.
		 */
		virtual void onWebViewCrashed(Awesomium::WebView* caller) = 0;
		
		/**
		 * This event is fired when the renderer for a certain plugin (usually 
		 * Flash, which is isolated in a separate process) crashes unexpectedly.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	pluginName	The name of the plugin that crashed.
		 */
		virtual void onPluginCrashed(Awesomium::WebView* caller, 
									 const std::wstring& pluginName) = 0;
		
		/**
		 * This event is fired when the page requests for the containing window 
		 * to be moved to a certain location on the screen.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	x	The x-coordinate.
		 *
		 * @param	y	The y-coordinate.
		 */
		virtual void onRequestMove(Awesomium::WebView* caller, 
								   int x, int y) = 0;
		
		/**
		 * This event is fired when the contents of the page has finished 
		 * loading. This occurs at the end of most page loads.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	url		The URL of the page.
		 *
		 * @param	contents	The contents of the page as plain text.
		 */
		virtual void onGetPageContents(Awesomium::WebView* caller, 
									   const std::string& url, 
									   const std::wstring& contents) = 0;
		
		/**
		 * This event is fired once the DOM (Document Object Model) for a page 
		 * is ready. This is very useful for executing Javascript on a page 
		 * before its content has finished loading.
		 *
		 * @param	caller	The WebView that fired the event.
		 */
		virtual void onDOMReady(Awesomium::WebView* caller) = 0;

		/**
		 * This event is fired whenever a page requests a file chooser dialog
		 * to be displayed (usually the result of an "input" element with type
		 * "file" being clicked by a user). You will need to display your own
		 * dialog (it does not have to be modal, this request does not block).
		 * Once a file has been chosen by the user, WebView::chooseFile 
		 * (or WebView::chooseMultipleFiles) should be called.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	selectMultipleFiles	Whether or not the dialog should allow
		 *								more than one file to be selected.
		 *
		 * @param	title	The title to be displayed (if any).
		 *
		 * @param	defaultPath	The default file path the dialog should
		 *						display (if any).
		 */
		virtual void onRequestFileChooser(Awesomium::WebView* caller,
										  bool selectMultipleFiles,
										  const std::wstring& title,
										  const std::wstring& defaultPath) = 0;

		/**
		* This event is fired as a response to WebView::requestScrollData.
		*
		* @param	caller	The WebView that fired the event.
		*
		* @param	contentWidth	The width of the contents of the page
		*                              (actual page width, NOT WebView width)
		*
		* @param	contentHeight	The height of the contents of the page
		*                              (actual page height, NOT WebView height)
		*
		* @param	preferredWidth	The preferred width of the page (usually the
		*                               minimum width specified via CSS)
		*
		* @param	scrollX			The current scroll position (x value)
		*
		* @param	scrollY			The current scroll position (y value)
		*/
		virtual void onGetScrollData(Awesomium::WebView* caller,
									 int contentWidth,
									 int contentHeight,
									 int preferredWidth,
									 int scrollX,
									 int scrollY) = 0;
		
		/**
		 * This event is fired whenever a message is added to the Javascript
		 * console. This is usually the result of a Javascript error being
		 * encountered on a page.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	message	The console message.
		 *
		 * @param	lineNumber	The line number that the message occured on.
		 *
		 * @param	source	The source (usually the filename) of the message.
		 */
		virtual void onJavascriptConsoleMessage(Awesomium::WebView* caller,
												const std::wstring& message,
												int lineNumber,
												const std::wstring& source) = 0;

		/**
		 * This event is fired whenever we get the results back from a
		 * call to WebView::find. You may receive multiple events as
		 * more results come in.
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	requestID	The unique ID of the request passed to WebView::find
		 *
		 * @param	numMatches	Number of matches on the page
		 *
		 * @param	selection	The location of the currently-selected match.
		 *
		 * @param	curMatch	The index of the current match.
		 *
		 * @param	finalUpdate	Whether or not this is the final update.
		 */
		virtual void onGetFindResults(Awesomium::WebView* caller,
                                      int requestID,
                                      int numMatches,
                                      const Awesomium::Rect& selection,
                                      int curMatch,
                                      bool finalUpdate) = 0;

		/**
		 * You should handle this message if you are displaying your
		 * own IME (input method editor) widget.
		 *
		 * This event is fired whenever the user does something that may change 
		 * the position or visiblity of the IME Widget (for example, deselecting 
		 * a textbox, scrolling the page, tabbing to another textbox, or clicking 
		 * the submit button). This event is only active when IME is activated
		 * (please see WebView::activateIME).
		 *
		 * @param	caller	The WebView that fired the event.
		 *
		 * @param	imeState	The action to apply to your IME widget (for
		 *						example, IME_DISABLE, IME_MOVE_WINDOW, and
		 *						IME_COMPLETE_COMPOSITION).
		 *
		 * @param	caretRect	The current location of the input caret on
		 *						the page. You should generally move your IME
		 *						widget to this location.
		 */
		virtual void onUpdateIME(Awesomium::WebView* caller,
                                 Awesomium::IMEState imeState,
                                 const Awesomium::Rect& caretRect) = 0;
	};
	
}

#endif