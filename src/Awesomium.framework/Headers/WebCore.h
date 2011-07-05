/**
 * @file WebCore.h
 *
 * @brief The main header for the Awesomium API.
 *
 * @author
 *
 * This file is a part of Awesomium, a library that makes it easy for 
 * developers to embed the web in their applications.
 *
 * Website: <http://www.khrona.com/products/awesomium>
 * 
 * Copyright (C) 2010 Khrona. All rights reserved. Awesomium is a 
 * trademark of Khrona.
 */

#ifndef __AWESOMIUM_WEB_CORE_H__
#define __AWESOMIUM_WEB_CORE_H__

#include <Awesomium/PlatformUtils.h>
#include <Awesomium/WebView.h>

class Profile;

namespace Awesomium {
	
	class WebViewImpl;
	class WebCoreImpl;
	class WebViewEvent;
	
	/**
	 * An enumeration of the three verbosity settings for the Awesomium Log.
	 */
	enum LogLevel
	{
		/// No log is created
		LOG_NONE,

		/// Logs only errors
		LOG_NORMAL,

		/// Logs everything
		LOG_VERBOSE		
	};
	
	/**
	* Configuration settings for the WebCore.
	*/
	class _OSMExport WebCoreConfig
	{
	public:
		/**
		 * Creates a default WebCoreConfig with the following default settings:
		 * 
		 * <pre>
		 *     enablePlugins           = false
		 *     enableJavascript        = true
		 *     userDataPath            = (empty)
		 *     pluginPath              = (empty)
		 *     logPath                 = (empty)
		 *     logLevel                = LOG_NORMAL
		 *     userAgentOverride       = (empty)
		 *     proxyServer             = "none"
		 *     proxyConfigScript       = (empty)
		 *     saveCacheAndCookies     = false
		 *     maxCacheSize            = 0
		 *     disableSameOriginPolicy = false
		 *     customCSS               = (empty)
		 * </pre>
		 */
		WebCoreConfig();
		
		/**
		 * Set whether or not to enable embedded plugins (e.g., Flash).
		 */
		void setEnablePlugins(bool enablePlugins);

		/**
		 * Set whether or not Javascript is enabled.
		 */
		void setEnableJavascript(bool enableJavascript);
		
		/** 
		 * Set the path to the directory that will be used to store cache, 
		 * cookies, and other data. If an empty string is specified, this path 
		 * defaults to "./Default".
		 */
		void setUserDataPath(const std::wstring& userDataPath);
		
		/** 
		 * Set the path to the directory that will be used to store cache, 
		 * cookies, and other data. If an empty string is specified, this path 
		 * defaults to "./Default".
		 */
		void setUserDataPath(const std::string& userDataPath);
		
		/**
		 * Set an absolute path that will be included in the search for plugins. 
		 * This is useful if you wish to bundle certain plugins with your 
		 * application.
		 */
		void setPluginPath(const std::wstring& pluginPath);
		
		/**
		 * Set an absolute path that will be included in the search for plugins. 
		 * This is useful if you wish to bundle certain plugins with your 
		 * application.
		 */
		void setPluginPath(const std::string& pluginPath);
		
		/**
		 * Set the path to store the awesomium.log. If none is specified, the
		 * log will be stored in the working directory.
		 */
		void setLogPath(const std::wstring& logPath);
		
		/**
		 * Set the path to store the awesomium.log. If none is specified, the
		 * log will be stored in the working directory.
		 */
		void setLogPath(const std::string& logPath);
		
		/** 
		 * Set the logging level to use, this can be either LOG_NONE, 
		 * LOG_NORMAL, or LOG_VERBOSE.
		 */
		void setLogLevel(LogLevel logLevel);
		
		/**
		 * Set the user agent string that will be used to override the default. 
		 * Leave this empty to use the default user agent.
		 */
		void setUserAgentOverride(const std::string& userAgentOverride);
		
		/**
		 * Set this to configure the proxy settings for all network requests.
		 * Specify "none" to disable all proxy use, specify "auto" to attempt
		 * to detect the proxy using system settings (e.g., via the 
		 * Internet Properties dialog on Windows or the Network panel of
		 * System Preferences on Mac OSX). Specify anything else to set
		 * manual proxy settings.
		 *
		 * <pre>
		 * For example:
		 * 
		 *    "none"                         -- No proxy. (Default).
		 *
		 *    "auto"                         -- Detect system proxy settings.
		 *
		 *    "http=myproxy:80;ftp=myproxy2" -- Use HTTP proxy "myproxy:80"  
		 *                                      for http:// URLs, and HTTP proxy 
		 *                                      "myproxy2:80" for ftp:// URLs.
		 *
		 *    "myproxy:80"                   -- Use HTTP proxy "foopy:80" for
		 *                                      all URLs.
		 *
		 *    "socks4://myproxy"             -- Use SOCKS v4 proxy "foopy:1080" 
		 *                                      for all URLs.
		 * </pre>
		 */
		void setProxyServer(const std::string& proxyServer);
		
		/**
		 * Set the URL to the PAC (Proxy Auto-Config) Script to use.
		 * See <http://en.wikipedia.org/wiki/Proxy_auto-config> for more info.
		 */
		void setProxyConfigScript(const std::string& proxyConfigScript);
		
		/**
		 * Set whether or not cache and cookies should be saved to disk.
		 * This is useful for pers
		 */
		void setSaveCacheAndCookies(bool saveCacheAndCookies);
		
		/**
		 * Set the maximum disk space to be used by the disk cache, in bytes.
		 * Specify 0 to use no limit.
		 */
		void setMaxCacheSize(int maxCacheSize);
		
		/**
		 * Set whether or not the "same-origin" policy should be disabled.
		 * See <http://en.wikipedia.org/wiki/Same_origin_policy>
		 */
		void setDisableSameOriginPolicy(bool disableSameOriginPolicy);
		
		/**
		 * Set a string of custom CSS to be included as the global default
		 * style for all pages. This is especially useful for customizing
		 * scrollbars and other look-and-feel elements.
		 */
		void setCustomCSS(const std::string& customCSS);

		/**
		 * Set a custom CSS file to be included as the global default style for 
		 * all pages. This is especially useful for customizing scrollbars and 
		 * other look-and-feel elements.
		 */
		void setCustomCSSFromFile(const std::string& customCSSFilePath);
	protected:

		bool enablePlugins;
		bool enableJavascript;
		std::wstring userDataPath;
		std::wstring pluginPath;
		std::wstring logPath;
		LogLevel logLevel;
		std::string userAgentOverride;
		std::string proxyServer;
		std::string proxyConfigScript;
		bool saveCacheAndCookies;
		int maxCacheSize;
		bool disableSameOriginPolicy;
		std::string customCSS;

		friend class WebCoreImpl;
		friend class WebCore;
	};
	
	/**
	 * The WebCore is the "core" of Awesomium-- it manages the lifetime of 
	 * all WebViews (see Awesomium::WebView) and maintains useful services 
	 * like resource caching and network connections.
	 * 
	 * Generally, you should create an instance of the WebCore at the 
	 * beginning of your program and then destroy the instance at the end 
	 * of your program.
	 */
	class _OSMExport WebCore
	{
	public:
		/**
		 * Instantiates the WebCore singleton (you can access it later with
		 * WebCore::Get or GetPointer).
		 *
		 * @param	config	Various configuration settings. See WebCoreConfig 
		 *                  for more information.
		 */
		WebCore(const WebCoreConfig& config = WebCoreConfig());
		
		/**
		 * Destroys the WebCore singleton. (Also destroys any current WebViews)
		 */
		~WebCore();
		
		/**
		 * Retrieves the WebCore singleton.
		 *
		 * @note	This will assert if the singleton is not instantiated.
		 *
		 * @return	Returns a reference to the instance.
		 */
		static WebCore& Get();
		
		/**
		 * Retrieves the WebCore singleton.
		 *
		 * @return	Returns a pointer to the instance.
		 */
		static WebCore* GetPointer();
		
		/**
		 * Sets the base directory.
		 *
		 * @param	baseDirectory	The absolute path to your base directory. 
		 *                          The base directory is a location that holds
		 *                          all of your local assets. It will be used 
		 *                          for WebView::loadFile and WebView::loadHTML
		 *                          (to resolve relative URLs).
		 */
		void setBaseDirectory(const std::string& baseDirectory);
		
		/**
		 * Sets the base directory.
		 *
		 * @param	baseDirectory	The absolute path to your base directory. 
		 *                          The base directory is a location that holds
		 *                          all of your local assets. It will be used 
		 *                          for WebView::loadFile and WebView::loadHTML
		 *                          (to resolve relative URLs).
		 */
		void setBaseDirectory(const std::wstring& baseDirectory);
		
		/**
		 * Creates a new WebView.
		 *
		 * @param	width	The width of the WebView in pixels.
		 * @param	height	The height of the WebView in pixels.
		 *
		 * @return	Returns a pointer to the created WebView.
		 */
		WebView* createWebView(int width, 
							   int height);
		
		/**
		 * Sets a custom response page to use when a WebView encounters a 
		 * certain HTML status code from the server (like '404 - File not found').
		 *
		 * @param	statusCode	The status code this response page should be 
		 *                      associated with.
		 *						See <http://en.wikipedia.org/wiki/List_of_HTTP_status_codes>
		 *
		 * @param	filePath	The local page to load as a response, should be
		 *                      a path relative to the base directory.
		 */
		void setCustomResponsePage(int statusCode, 
								   const std::wstring& filePath);
		
		/**
		 * Updates the WebCore and allows it to conduct various operations such
		 * as updating the render buffer of each WebView, destroying any
		 * WebViews that are queued for destruction, and invoking any queued
		 * WebViewListener events.
		 */
		void update();
		
		/**
		 * Retrieves the base directory.
		 *
		 * @return	Returns the current base directory.
		 */
		const std::wstring& getBaseDirectory() const;
		
		/**
		 * Returns whether or not plugins are enabled.
		 */
		bool arePluginsEnabled() const;
		
		/**
		 * Clear the disk cache and media cache.
		 */
		void clearCache();
		
		/**
		 * Clear all cookies.
		 */
		void clearCookies();
		
		/**
		 * Sets a cookie for a certain URL.
		 * 
		 * @param	url	The URL to set the cookie on.
		 *
		 * @param	cookieString	The cookie string, for example:
		 *                          <pre> "key1=value1; key2=value2" </pre>
		 *
		 * @param	isHTTPOnly	Whether or not this cookie is HTTP-only.
		 *
		 * @param	forceSessionCookie	Whether or not to force this as a 
		 *	                            session cookie.
		 *
		 * @return	Returns true if the cookie was successfully set, false
		 *          otherwise.
		 */
		bool setCookie(const std::string& url, 
					   const std::string& cookieString, 
					   bool isHTTPOnly = false, 
					   bool forceSessionCookie = false);

		/**
		 * Gets all cookies for a certain URL.
		 *
		 * @param	url	The URL whose cookies will be retrieved.
		 *
		 * @param	excludeHTTPOnly	Whether or not to exclude HTTP-only
		 *                          cookies from the result.
		 *
		 * @return	Returns the cookie string.
		 */
		const std::string& getCookies(const std::string& url, 
									  bool excludeHTTPOnly = true);

		/**
		 * Deletes a certain cookie on a certain URL.
		 * 
		 * @param	url	The URL that we will be deleting cookies on.
		 *
		 * @param	cookieName	The name of the cookie that will be deleted.
		 */
		void deleteCookie(const std::string& url, 
						  const std::string& cookieName);

		/**
		 * Set whether or not the printer dialog should be suppressed or not.
		 * Set this to "true" to hide printer dialogs and print immediately
		 * using the OS's default printer when WebView::print is called.
		 * Default is "false" if you never call this.
		 *
		 * @param	suppress	Whether or not the printer dialog should be 
		 *                      suppressed.
		 */
		void setSuppressPrinterDialog(bool suppress);

	protected:
		
		Profile* getProfile();
		bool resolveFutureJSValue(WebView* view, int requestID, JSValue& value, 
								  int timeoutMs);
		void enqueueWebViewEvent(WebViewEvent* webViewEvent);
		const std::string& getCustomCSS();
		
		static WebCore* instance;
		WebCoreImpl* webCoreImpl;
		std::wstring baseDirectory;
		bool pluginsEnabled;
		std::string customCSS;
		std::string getCookiesCache;
		
		friend class WebViewImpl;
		friend class FutureJSValue;
	};
}

/**
 * @mainpage Awesomium C++ API
 *
 * @section intro_sec Introduction
 *
 * Hi there, welcome to the Awesomium C++ API docs! Awesomium is a software 
 * library that makes it easy to put the web in your applications. Whether 
 * that means embedded web browsing, rendering pages as images, streaming 
 * pages over the net, or manipulating web content live for some other 
 * purpose, Awesomium does it all.
 *
 * If this is your first time exploring the API, we recommend
 * starting with Awesomium::WebCore and Awesomium::WebView.
 *
 * For more help and tips with the API, please visit our Knowledge Base
 *     <http://support.awesomium.com/faqs>
 *
 * @section usefullinks_sec Useful Links
 * - Awesomium Main: <http://www.awesomium.com>
 * - Support Home: <http://support.awesomium.com>
 * 
 * @section copyright_sec Copyright
 * This documentation is copyright (C) 2011 Khrona. All rights reserved. 
 * Awesomium is a trademark of Khrona.
 */

#endif