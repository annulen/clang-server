all: tgt1 tgt2 tgt3 tgt4 tgt5 tgt6 tgt7 tgt8 

tgt1:
	clang-client ~/sdsd "rtrtrt ghhghg" ujkjkj -c qwqwq 1 

tgt2:
	clang-client dsd trtrt hhghg jkjkj -c wqwq 2

tgt3: tgt9
	clang-client sdsd rtrtrt ghhghg ujkjkj -c qwqwq 3 

tgt4:
	clang-client dsd trtrt hhghg jkjkj -c wqwq 4 

tgt5:
	clang-client sdsd rtrtrt ghhghg ujkjkj -c qwqwq 5 

tgt6:
	clang-client dsd trtrt hhghg jkjkj -c wqwq 6 

tgt7:
	clang-client sdsd rtrtrt ghhghg ujkjkj -c qwqwq 7 

tgt8:
	clang-client dsd trtrt hhghg jkjkj -c wqwq 8 

tgt9:
	@clang++-client -c -pipe -m64 -Wall -Wextra -Wreturn-type -fno-strict-aliasing -Wcast-align -Wchar-subscripts -Wformat-security -Wreturn-type -Wno-unused-parameter -Wno-sign-compare -Wno-switch -Wno-switch-enum -Wundef -Wmissing-noreturn -Winit-self -g -ffunction-sections -fdata-sections -O2 -fvisibility=hidden -fvisibility-inlines-hidden -D_REENTRANT -fPIC -DENABLE_3D_RENDERING=0 -DENABLE_CHANNEL_MESSAGING=0 -DENABLE_DATABASE=0 -DENABLE_DATALIST=0 -DENABLE_DOM_STORAGE=0 -DENABLE_EVENTSOURCE=0 -DENABLE_FILTERS=0 -DENABLE_ICONDATABASE=0 -DENABLE_JAVASCRIPT_DEBUGGER=0 -DENABLE_NOTIFICATIONS=0 -DENABLE_OFFLINE_WEB_APPLICATIONS=0 -DENABLE_RUBY=0 -DENABLE_SANDBOX=0 -DENABLE_SHARED_WORKERS=0 -DENABLE_SVG=0 -DENABLE_SVG_ANIMATION=0 -DENABLE_SVG_FONTS=0 -DENABLE_SVG_FOREIGN_OBJECT=0 -DENABLE_SVG_USE=0 -DENABLE_TILED_BACKING_STORE=0 -DBUILDING_QT__=1 -DWTF_USE_ACCELERATED_COMPOSITING -DNDEBUG -DUSE_SYSTEM_MALLOC -DENABLE_JIT=0 -DQT_MAKEDLL -DBUILD_WEBKIT -DENABLE_JIT=0 -DBUILDING_QT__ -DBUILDING_JavaScriptCore -DBUILDING_WTF -DENABLE_JIT=0 -DENABLE_NETSCAPE_PLUGIN_API=0 -DENABLE_XSLT=0 -DENABLE_DIRECTORY_UPLOAD=0 -DENABLE_SQLITE=0 -DENABLE_DASHBOARD_SUPPORT=0 -DENABLE_XPATH=1 -DENABLE_WCSS=0 -DENABLE_WML=0 -DENABLE_WORKERS=1 -DENABLE_XHTMLMP=0 -DENABLE_DATAGRID=0 -DENABLE_METER_TAG=1 -DENABLE_PROGRESS_TAG=1 -DENABLE_BLOB_SLICE=0 -DENABLE_IMAGE_RESIZER=0 -DENABLE_INPUT_SPEECH=0 -DENABLE_HAPTICS=1 -DENABLE_SVG_FONTS=0 -DENABLE_SVG_FOREIGN_OBJECT=0 -DENABLE_SVG_ANIMATION=0 -DENABLE_SVG_AS_IMAGE=0 -DENABLE_SVG_USE=0 -DENABLE_WEB_SOCKETS=1 -DENABLE_WEB_TIMING=0 -DENABLE_QT_BEARER=1 -DENABLE_TOUCH_EVENTS=1 -DENABLE_VIDEO=0 -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED -I/usr/local/Trolltech/Qt-4.7.3-qws/mkspecs/qws/linux-x86_64-g++ -I../../../WebCore -I/usr/local/Trolltech/Qt-4.7.3-qws/include/QtCore -I/usr/local/Trolltech/Qt-4.7.3-qws/include/QtNetwork -I/usr/local/Trolltech/Qt-4.7.3-qws/include/QtGui -I/usr/local/Trolltech/Qt-4.7.3-qws/include -I../../../WebCore/bridge/qt -I../../../WebCore/page/qt -I../../../WebCore/platform/graphics/qt -I../../../WebCore/platform/network/qt -I../../../WebCore/platform/qt -I../../../WebKit/qt/Api -I../../../WebKit/qt/WebCoreSupport -I../../../WebCore -I../../../WebCore/accessibility -I../../../WebCore/bindings -I../../../WebCore/bindings/generic -I../../../WebCore/bindings/js -I../../../WebCore/bridge -I../../../WebCore/bridge/c -I../../../WebCore/bridge/jsc -I../../../WebCore/css -I../../../WebCore/dom -I../../../WebCore/dom/default -I../../../WebCore/editing -I../../../WebCore/history -I../../../WebCore/html -I../../../WebCore/html/canvas -I../../../WebCore/inspector -I../../../WebCore/loader -I../../../WebCore/loader/appcache -I../../../WebCore/loader/archive -I../../../WebCore/loader/icon -I../../../WebCore/mathml -I../../../WebCore/notifications -I../../../WebCore/page -I../../../WebCore/page/animation -I../../../WebCore/platform -I../../../WebCore/platform/animation -I../../../WebCore/platform/graphics -I../../../WebCore/platform/graphics/filters -I../../../WebCore/platform/graphics/transforms -I../../../WebCore/platform/image-decoders -I../../../WebCore/platform/mock -I../../../WebCore/platform/network -I../../../WebCore/platform/sql -I../../../WebCore/platform/text -I../../../WebCore/platform/text/transcoder -I../../../WebCore/plugins -I../../../WebCore/rendering -I../../../WebCore/rendering/style -I../../../WebCore/storage -I../../../WebCore/svg -I../../../WebCore/svg/animation -I../../../WebCore/svg/graphics -I../../../WebCore/svg/graphics/filters -I../../../WebCore/websockets -I../../../WebCore/wml -I../../../WebCore/workers -I../../../WebCore/xml -Igenerated -I../../../JavaScriptCore -I../../../../WebKit -I../../../JavaScriptCore/assembler -I../../../JavaScriptCore/bytecode -I../../../JavaScriptCore/bytecompiler -I../../../JavaScriptCore/debugger -I../../../JavaScriptCore/interpreter -I../../../JavaScriptCore/jit -I../../../JavaScriptCore/parser -I../../../JavaScriptCore/pcre -I../../../JavaScriptCore/profiler -I../../../JavaScriptCore/runtime -I../../../JavaScriptCore/wtf -I/home/kostya/projects/git/WebKit/JavaScriptCore/wtf/symbian -I../../../JavaScriptCore/wtf/unicode -I../../../JavaScriptCore/yarr -I../../../JavaScriptCore/API -I../../../JavaScriptCore/ForwardingHeaders -I../JavaScriptCore/generated -I../include/QtWebKit -I. -I../../../WebCore -I. -o obj/release/CSSRule.o ../../../WebCore/css/CSSRule.cpp
