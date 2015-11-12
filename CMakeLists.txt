#/****************************************************************************
# Copyright (c) 2013-2014 cocos2d-x.org
#
# http://www.cocos2d-x.org
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# ****************************************************************************/
cmake_policy(SET CMP0017 NEW)

cmake_minimum_required(VERSION 2.8)

set(APP_NAME MyGame)
project (${APP_NAME})

set(COCOS2D_ROOT ${CMAKE_SOURCE_DIR}/cocos2d)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${COCOS2D_ROOT}/cmake/Modules/")
include(CocosBuildHelpers)

# libcocos2d
set(BUILD_CPP_TESTS OFF CACHE BOOL "turn off build cpp-tests")
set(BUILD_LUA_LIBS OFF CACHE BOOL "turn off build lua related targets")
set(BUILD_JS_LIBS OFF CACHE BOOL "turn off build js related targets")
add_subdirectory(${COCOS2D_ROOT})

# Some macro definitions
if(WINDOWS)
  if(BUILD_SHARED_LIBS)
    ADD_DEFINITIONS (-D_USRDLL -D_EXPORT_DLL_ -D_USEGUIDLL -D_USREXDLL -D_USRSTUDIODLL)
  else()
    ADD_DEFINITIONS (-DCC_STATIC)
  endif()

  ADD_DEFINITIONS (-DCOCOS2DXWIN32_EXPORTS -D_WINDOWS -DWIN32 -D_WIN32)
  set(PLATFORM_FOLDER win32)
elseif(MACOSX OR APPLE)
  ADD_DEFINITIONS (-DCC_TARGET_OS_MAC)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER mac)
elseif(LINUX)
  ADD_DEFINITIONS(-DLINUX)
  set(PLATFORM_FOLDER linux)
elseif(ANDROID)
  ADD_DEFINITIONS (-DUSE_FILE32API)
  set(PLATFORM_FOLDER android)
else()
  message( FATAL_ERROR "Unsupported platform, CMake will exit" )
endif()


# Compiler options
if(MSVC)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS
                  -wd4251 -wd4244 -wd4334 -wd4005 -wd4820 -wd4710
                  -wd4514 -wd4056 -wd4996 -wd4099)
else()
  set(CMAKE_C_FLAGS_DEBUG "-g -Wall -DCOCOS2D_DEBUG=1")
  set(CMAKE_CXX_FLAGS_DEBUG ${CMAKE_C_FLAGS_DEBUG})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wno-deprecated-declarations -Wno-reorder")
  if(CLANG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
  endif()
endif(MSVC)



set(PLATFORM_SPECIFIC_SRC)
set(PLATFORM_SPECIFIC_HEADERS)
if(MACOSX OR APPLE)
set(PLATFORM_SPECIFIC_SRC
  proj.ios_mac/ios/main.m
  proj.ios_mac/ios/RootViewController.mm
  proj.ios_mac/ios/AppController.mm
)
set(PLATFORM_SPECIFIC_HEADERS
  proj.ios_mac/ios/RootViewController.h
  proj.ios_mac/ios/AppController.h
)
elseif(LINUX) #assume linux
set(PLATFORM_SPECIFIC_SRC
  proj.linux/main.cpp
)
elseif ( WIN32 )
set(PLATFORM_SPECIFIC_SRC
  proj.win32/main.cpp
)
set(PLATFORM_SPECIFIC_HEADERS
  proj.win32/main.h
  proj.win32/resource.h
)
endif()

include_directories(
  /usr/local/include/GLFW
  /usr/include/GLFW
  ${COCOS2D_ROOT}/cocos
  ${COCOS2D_ROOT}/cocos/platform
  ${COCOS2D_ROOT}/cocos/audio/include/
  Classes
  Classes/others
  Classes/others/muduo
  Classes/others/muduo/base
  #Classes/others/muduo/base/tests
  Classes/others/muduo/net
  Classes/others/muduo/net/http
  #Classes/others/muduo/net/http/tests
  Classes/others/muduo/net/inspect
  #Classes/others/muduo/net/inspect/tests
  Classes/others/muduo/net/poller
  #Classes/others/muduo/net/protobuf
  #Classes/others/muduo/net/protorpc
  #Classes/others/muduo/net/tests
  Classes/others/muduo/other
  #Classes/others/muduo/other/gtest
  #Classes/others/muduo/other/gtest/internal
  #Classes/others/muduo/other/gtest/lib
  Classes/others/muduo/other/zlib
  Classes/others/muduo/other/zlib/inc
  Classes/others/muduo/other/zlib/lib
)
if ( WIN32 )
  include_directories(
  ${COCOS2D_ROOT}/external/glfw3/include/win32
  ${COCOS2D_ROOT}/external/win32-specific/gles/include/OGLES
)
endif( WIN32 )

set(GAME_SRC
  Classes/AppDelegate.cpp
  Classes/HelloWorldScene.cpp
  Classes/mytcpclient.cpp
  Classes/constants.cpp
  Classes/client.cc
  Classes/others/muduo/base/AsyncLogging.cc
  Classes/others/muduo/base/Condition.cc
  Classes/others/muduo/base/CountDownLatch.cc
  Classes/others/muduo/base/Date.cc
  Classes/others/muduo/base/Exception.cc
  Classes/others/muduo/base/FileUtil.cc
  Classes/others/muduo/base/LogFile.cc
  Classes/others/muduo/base/LogStream.cc
  Classes/others/muduo/base/Logging.cc
  Classes/others/muduo/base/ProcessInfo.cc
  Classes/others/muduo/base/Thread.cc
  Classes/others/muduo/base/ThreadPool.cc
  Classes/others/muduo/base/TimeZone.cc
  Classes/others/muduo/base/Timestamp.cc
  #Classes/others/muduo/base/tests/AsyncLogging_test.cc
  #Classes/others/muduo/base/tests/Atomic_unittest.cc
  #Classes/others/muduo/base/tests/BlockingQueue_bench.cc
  #Classes/others/muduo/base/tests/BlockingQueue_test.cc
  #Classes/others/muduo/base/tests/Date_unittest.cc
  #Classes/others/muduo/base/tests/Exception_test.cc
  #Classes/others/muduo/base/tests/FileUtil_test.cc
  #Classes/others/muduo/base/tests/Fork_test.cc
  #Classes/others/muduo/base/tests/GzipFile_test.cc
  #Classes/others/muduo/base/tests/LogFile_test.cc
  #Classes/others/muduo/base/tests/LogStream_bench.cc
  #Classes/others/muduo/base/tests/LogStream_test.cc
  #Classes/others/muduo/base/tests/Logging_test.cc
  #Classes/others/muduo/base/tests/Mutex_test.cc
  #Classes/others/muduo/base/tests/ProcessInfo_test.cc
  #Classes/others/muduo/base/tests/SingletonThreadLocal_test.cc
  #Classes/others/muduo/base/tests/Singleton_test.cc
  #Classes/others/muduo/base/tests/ThreadLocalSingleton_test.cc
  #Classes/others/muduo/base/tests/ThreadLocal_test.cc
  #Classes/others/muduo/base/tests/ThreadPool_test.cc
  #Classes/others/muduo/base/tests/Thread_bench.cc
  #Classes/others/muduo/base/tests/Thread_test.cc
  #Classes/others/muduo/base/tests/TimeZone_unittest.cc
  #Classes/others/muduo/base/tests/Timestamp_unittest.cc
  Classes/others/muduo/net/Acceptor.cc
  Classes/others/muduo/net/Buffer.cc
  Classes/others/muduo/net/Channel.cc
  Classes/others/muduo/net/Connector.cc
  Classes/others/muduo/net/EventLoop.cc
  Classes/others/muduo/net/EventLoopThread.cc
  Classes/others/muduo/net/EventLoopThreadPool.cc
  Classes/others/muduo/net/InetAddress.cc
  Classes/others/muduo/net/Poller.cc
  Classes/others/muduo/net/Socket.cc
  Classes/others/muduo/net/SocketsOps.cc
  Classes/others/muduo/net/TcpClient.cc
  Classes/others/muduo/net/TcpConnection.cc
  Classes/others/muduo/net/TcpServer.cc
  Classes/others/muduo/net/Timer.cc
  Classes/others/muduo/net/TimerQueue.cc
  Classes/others/muduo/net/http/HttpResponse.cc
  Classes/others/muduo/net/http/HttpServer.cc
  #Classes/others/muduo/net/http/tests/HttpRequest_unittest.cc
  #Classes/others/muduo/net/http/tests/HttpServer_test.cc
  Classes/others/muduo/net/inspect/Inspector.cc
  Classes/others/muduo/net/inspect/PerformanceInspector.cc
  Classes/others/muduo/net/inspect/ProcessInspector.cc
  Classes/others/muduo/net/inspect/SystemInspector.cc
  #Classes/others/muduo/net/inspect/tests/Inspector_test.cc
  Classes/others/muduo/net/poller/DefaultPoller.cc
  Classes/others/muduo/net/poller/EPollPoller.cc
  Classes/others/muduo/net/poller/PollPoller.cc
  #Classes/others/muduo/net/protobuf/ProtobufCodecLite.cc
  #Classes/others/muduo/net/protorpc/RpcChannel.cc
  #Classes/others/muduo/net/protorpc/RpcCodec.cc
  #Classes/others/muduo/net/protorpc/RpcCodec_test.cc
  #Classes/others/muduo/net/protorpc/RpcServer.cc
  #Classes/others/muduo/net/tests/Buffer_unittest.cc
  #Classes/others/muduo/net/tests/EchoServer_unittest.cc
  #Classes/others/muduo/net/tests/EventLoopThreadPool_unittest.cc
  #Classes/others/muduo/net/tests/EventLoopThread_unittest.cc
  #Classes/others/muduo/net/tests/EventLoop_unittest.cc
  #Classes/others/muduo/net/tests/InetAddress_unittest.cc
  #Classes/others/muduo/net/tests/TcpClient_reg1.cc
  #Classes/others/muduo/net/tests/TcpClient_reg2.cc
  #Classes/others/muduo/net/tests/TcpClient_reg3.cc
  #Classes/others/muduo/net/tests/TimerQueue_unittest.cc
  #Classes/others/muduo/net/tests/ZlibStream_unittest.cc
  ${PLATFORM_SPECIFIC_SRC}
)

set(GAME_HEADERS
  Classes/AppDelegate.h
  Classes/HelloWorldScene.h
  Classes/mytcpclient.h
  Classes/constants.h
  Classes/others/muduo/base/AsyncLogging.h
  Classes/others/muduo/base/Atomic.h
  Classes/others/muduo/base/BlockingQueue.h
  Classes/others/muduo/base/Condition.h
  Classes/others/muduo/base/CountDownLatch.h
  Classes/others/muduo/base/CurrentThread.h
  Classes/others/muduo/base/Date.h
  Classes/others/muduo/base/Exception.h
  Classes/others/muduo/base/FileUtil.h
  Classes/others/muduo/base/GzipFile.h
  Classes/others/muduo/base/LogFile.h
  Classes/others/muduo/base/LogStream.h
  Classes/others/muduo/base/Logging.h
  Classes/others/muduo/base/Mutex.h
  Classes/others/muduo/base/ProcessInfo.h
  Classes/others/muduo/base/Singleton.h
  Classes/others/muduo/base/StringPiece.h
  Classes/others/muduo/base/Thread.h
  Classes/others/muduo/base/ThreadLocal.h
  Classes/others/muduo/base/ThreadLocalSingleton.h
  Classes/others/muduo/base/ThreadPool.h
  Classes/others/muduo/base/TimeZone.h
  Classes/others/muduo/base/Timestamp.h
  Classes/others/muduo/base/Types.h
  Classes/others/muduo/base/WeakCallback.h
  Classes/others/muduo/base/copyable.h
  Classes/others/muduo/net/Acceptor.h
  Classes/others/muduo/net/Buffer.h
  Classes/others/muduo/net/Callbacks.h
  Classes/others/muduo/net/Channel.h
  Classes/others/muduo/net/Connector.h
  Classes/others/muduo/net/Endian.h
  Classes/others/muduo/net/EventLoop.h
  Classes/others/muduo/net/EventLoopThread.h
  Classes/others/muduo/net/EventLoopThreadPool.h
  Classes/others/muduo/net/InetAddress.h
  Classes/others/muduo/net/Poller.h
  Classes/others/muduo/net/Socket.h
  Classes/others/muduo/net/SocketsOps.h
  Classes/others/muduo/net/TcpClient.h
  Classes/others/muduo/net/TcpConnection.h
  Classes/others/muduo/net/TcpServer.h
  Classes/others/muduo/net/Timer.h
  Classes/others/muduo/net/TimerId.h
  Classes/others/muduo/net/TimerQueue.h
  Classes/others/muduo/net/ZlibStream.h
  Classes/others/muduo/net/http/HttpContext.h
  Classes/others/muduo/net/http/HttpRequest.h
  Classes/others/muduo/net/http/HttpResponse.h
  Classes/others/muduo/net/http/HttpServer.h
  Classes/others/muduo/net/inspect/Inspector.h
  Classes/others/muduo/net/inspect/PerformanceInspector.h
  Classes/others/muduo/net/inspect/ProcessInspector.h
  Classes/others/muduo/net/inspect/SystemInspector.h
  Classes/others/muduo/net/poller/EPollPoller.h
  Classes/others/muduo/net/poller/PollPoller.h
  #Classes/others/muduo/net/protobuf/BufferStream.h
  #Classes/others/muduo/net/protobuf/ProtobufCodecLite.h
  #Classes/others/muduo/net/protorpc/RpcChannel.h
  #Classes/others/muduo/net/protorpc/RpcCodec.h
  #Classes/others/muduo/net/protorpc/RpcServer.h
  #Classes/others/muduo/net/protorpc/google-inl.h
  Classes/others/muduo/other/any.h
  #Classes/others/muduo/other/gtest/gtest-death-test.h
  #Classes/others/muduo/other/gtest/gtest-message.h
  #Classes/others/muduo/other/gtest/gtest-param-test.h
  #Classes/others/muduo/other/gtest/gtest-printers.h
  #Classes/others/muduo/other/gtest/gtest-spi.h
  #Classes/others/muduo/other/gtest/gtest-test-part.h
  #Classes/others/muduo/other/gtest/gtest-typed-test.h
  #Classes/others/muduo/other/gtest/gtest.h
  #Classes/others/muduo/other/gtest/gtest_pred_impl.h
  #Classes/others/muduo/other/gtest/gtest_prod.h
  #Classes/others/muduo/other/gtest/internal/gtest-death-test-internal.h
  #Classes/others/muduo/other/gtest/internal/gtest-filepath.h
  #Classes/others/muduo/other/gtest/internal/gtest-internal.h
  #Classes/others/muduo/other/gtest/internal/gtest-linked_ptr.h
  #Classes/others/muduo/other/gtest/internal/gtest-param-util-generated.h
  #Classes/others/muduo/other/gtest/internal/gtest-param-util.h
  #Classes/others/muduo/other/gtest/internal/gtest-port.h
  #Classes/others/muduo/other/gtest/internal/gtest-string.h
  #Classes/others/muduo/other/gtest/internal/gtest-tuple.h
  #Classes/others/muduo/other/gtest/internal/gtest-type-util.h
  Classes/others/muduo/other/noncopyable.h
  Classes/others/muduo/other/zlib/inc/crc32.h
  Classes/others/muduo/other/zlib/inc/deflate.h
  Classes/others/muduo/other/zlib/inc/gzguts.h
  Classes/others/muduo/other/zlib/inc/inffast.h
  Classes/others/muduo/other/zlib/inc/inffixed.h
  Classes/others/muduo/other/zlib/inc/inflate.h
  Classes/others/muduo/other/zlib/inc/inftrees.h
  Classes/others/muduo/other/zlib/inc/trees.h
  Classes/others/muduo/other/zlib/inc/zconf.h
  Classes/others/muduo/other/zlib/inc/zlib.h
  Classes/others/muduo/other/zlib/inc/zutil.h
  ${PLATFORM_SPECIFIC_HEADERS}
)

if(GAME_HEADERS)
  if ( WIN32 )
    add_executable(${APP_NAME} WIN32 ${GAME_SRC} ${GAME_HEADERS})
  else()
    add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEADERS})
  endif ( WIN32 )
else()
  if ( WIN32 )
    add_executable(${APP_NAME} WIN32 ${GAME_SRC})
  else()
    add_executable(${APP_NAME} ${GAME_SRC})
  endif ( WIN32 )
endif()

target_link_libraries(${APP_NAME} cocos2d)

set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin")

set_target_properties(${APP_NAME} PROPERTIES
     RUNTIME_OUTPUT_DIRECTORY  "${APP_BIN_DIR}")

if ( WIN32 )
  #also copying dlls to binary directory for the executable to run
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/gles/prebuilt/glew32.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	COMMAND ${CMAKE_COMMAND} -E copy ${COCOS2D_ROOT}/external/win32-specific/zlib/prebuilt/zlib1.dll ${APP_BIN_DIR}/${CMAKE_BUILD_TYPE}
	)
else()
  pre_build(${APP_NAME}
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${APP_BIN_DIR}/Resources
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/Resources ${APP_BIN_DIR}/Resources
    )

endif()
