#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <evpp/libevent.h>
#include <evpp/timestamp.h>
#include <evpp/event_loop_thread.h>

#include <evpp/httpc/request.h>
#include <evpp/httpc/conn.h>
#include <evpp/httpc/response.h>

#include "evpp/http/service.h"
#include "evpp/http/context.h"
#include "evpp/http/http_server.h"

#define H_TEST_ASSERT assert

static bool g_stopping = false;
static void RequestHandler(evpp::EventLoop* loop, const evpp::http::ContextPtr& ctx, const evpp::http::HTTPSendResponseCallback& cb) {
    std::stringstream oss;
    oss << "func=" << __FUNCTION__ << " OK"
        << " ip=" << ctx->remote_ip() << "\n"
        << " uri=" << ctx->uri() << "\n"
        << " body=" << ctx->body().ToString() << "\n";
    cb(oss.str());
}

static void RequestHandler201(evpp::EventLoop* loop, const evpp::http::ContextPtr& ctx, const evpp::http::HTTPSendResponseCallback& cb) {
    std::stringstream oss;
    oss << "func=" << __FUNCTION__ << " OK"
        << " ip=" << ctx->remote_ip() << "\n"
        << " uri=" << ctx->uri() << "\n"
        << " body=" << ctx->body().ToString() << "\n";
    ctx->set_response_http_code(201);
    cb(oss.str());
}

static void RequestHandler909(evpp::EventLoop* loop, const evpp::http::ContextPtr& ctx, const evpp::http::HTTPSendResponseCallback& cb) {
    LOG_INFO << "RequestHandler909";
    std::stringstream oss;
    oss << "func=" << __FUNCTION__ << " OK"
        << " ip=" << ctx->remote_ip() << "\n"
        << " uri=" << ctx->uri() << "\n"
        << " body=" << ctx->body().ToString() << "\n";
    ctx->set_response_http_code(909);
    cb(oss.str());
}

static void DefaultRequestHandler(evpp::EventLoop* loop, const evpp::http::ContextPtr& ctx, const evpp::http::HTTPSendResponseCallback& cb) {
    //std::cout << __func__ << " called ...\n";
    std::stringstream oss;
    oss << "func=" << __FUNCTION__ << "\n"
        << " ip=" << ctx->remote_ip() << "\n"
        << " uri=" << ctx->uri() << "\n"
        << " body=" << ctx->body().ToString() << "\n";

    if (ctx->uri().find("stop") != std::string::npos) {
        g_stopping = true;
    }

    cb(oss.str());
}

static void RequestHandlerHTTPClientRetry(evpp::EventLoop* loop, const evpp::http::ContextPtr& ctx, const evpp::http::HTTPSendResponseCallback& cb) {
    std::stringstream oss;
    oss << "func=" << __FUNCTION__ << " OK"
        << " ip=" << ctx->remote_ip() << "\n"
        << " uri=" << ctx->uri() << "\n"
        << " body=" << ctx->body().ToString() << "\n";
    static std::atomic<int> i(0);
    if (i++ == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    cb(oss.str());
}

namespace {

static std::vector<int> g_listening_port = { 49000, 49001 };

static std::string GetHttpServerURL() {
    static int i = 0;
    std::ostringstream oss;
    oss << "http://127.0.0.1:" << g_listening_port[(i++ % g_listening_port.size())];
    return oss.str();
}

void testDefaultHandler1(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/status?a=1";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, "", evpp::Duration(10.0));
    auto f = [r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(result.find("uri=/status") != std::string::npos);
        H_TEST_ASSERT(result.find("uri=/status?a=1") == std::string::npos);
        H_TEST_ASSERT(result.find("func=DefaultRequestHandler") != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

void testDefaultHandler2(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/status";
    std::string body = "The http request body.";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, body, evpp::Duration(10.0));
    auto f = [body, r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(result.find("uri=/status") != std::string::npos);
        H_TEST_ASSERT(result.find("func=DefaultRequestHandler") != std::string::npos);
        H_TEST_ASSERT(result.find(body.c_str()) != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

void testDefaultHandler3(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/status/method/method2/xx";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, "", evpp::Duration(10.0));
    auto f = [r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(result.find("uri=/status/method/method2/xx") != std::string::npos);
        H_TEST_ASSERT(result.find("func=DefaultRequestHandler") != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

void testPushBootHandler(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/push/boot";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, "", evpp::Duration(10.0));
    auto f = [r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(result.find("uri=/push/boot") != std::string::npos);
        H_TEST_ASSERT(result.find("func=RequestHandler") != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

void testRequestHandler201(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/201";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, "", evpp::Duration(10.0));
    auto f = [r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(response->http_code() == 201);
        H_TEST_ASSERT(result.find("uri=/201") != std::string::npos);
        H_TEST_ASSERT(result.find("func=RequestHandler201") != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

void testRequestHandler909(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/909";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, "", evpp::Duration(10.0));
    auto f = [r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(response->http_code() == 909);
        H_TEST_ASSERT(result.find("uri=/909") != std::string::npos);
        H_TEST_ASSERT(result.find("func=RequestHandler909") != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

void testRequestHandlerUriPathAndParam(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/UriPathAndParam?key2=key2value&key1=key1value";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, "", evpp::Duration(10.0));
    auto f = [r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(response->http_code() == 200);
        H_TEST_ASSERT(result.find("uri=/UriPathAndParam") != std::string::npos);
        H_TEST_ASSERT(result.find("key2=key2value") != std::string::npos);
        H_TEST_ASSERT(result.find("key1=key1value") != std::string::npos);
        H_TEST_ASSERT(result.find("notkey=\n") != std::string::npos);
        H_TEST_ASSERT(result.find("func=RequestHandlerUriPathAndParam") != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

void testStop(evpp::EventLoop* loop, int* finished) {
    std::string uri = "/mod/stop";
    std::string url = GetHttpServerURL() + uri;
    auto r = new evpp::httpc::Request(loop, url, "", evpp::Duration(10.0));
    auto f = [r, finished](const std::shared_ptr<evpp::httpc::Response>& response) {
        std::string result = response->body().ToString();
        H_TEST_ASSERT(!result.empty());
        H_TEST_ASSERT(result.find("uri=/mod/stop") != std::string::npos);
        H_TEST_ASSERT(result.find("func=DefaultRequestHandler") != std::string::npos);
        *finished += 1;
        delete r;
    };

    r->Execute(f);
}

static void TestAll() {
    evpp::EventLoopThread t;
    t.Start(true);
    int finished = 0;
    testDefaultHandler1(t.loop(), &finished);
    testDefaultHandler2(t.loop(), &finished);
    testDefaultHandler3(t.loop(), &finished);
    testPushBootHandler(t.loop(), &finished);
    testRequestHandler201(t.loop(), &finished);
    testRequestHandler909(t.loop(), &finished);
    testStop(t.loop(), &finished);

    while (true) {
        usleep(10);

        if (finished == 7) {
            break;
        }
    }

    t.Stop(true);
}

static void Test909() {
    evpp::EventLoopThread t;
    t.Start(true);
    int finished = 0;
    testRequestHandler909(t.loop(), &finished);
    testStop(t.loop(), &finished);

    while (true) {
        usleep(10);

        if (finished == 2) {
            break;
        }
    }

    t.Stop(true);
}

}

int main() {
    int i = 2;
    LOG_INFO << "Running testHTTPServer i=" << i;
    evpp::http::Server ph(i);
    ph.RegisterDefaultHandler(&DefaultRequestHandler);
    ph.RegisterHandler("/909", &RequestHandler909);
    bool r = ph.Init(g_listening_port);
    auto pid = fork();
    if (pid != 0) {
        // In parent process 
        LOG_INFO << "In parent process. Starting";
        ph.Start();
        LOG_INFO << "In parent process. Stopping";
        ph.Stop();
        LOG_INFO << "In parent process. Stopped";
        return 0;
    }
    
    LOG_INFO << "In child process. Doing AfterFork";
    ph.AfterFork();
    ph.Start();

    if(!r)
        LOG_ERROR << "ph.Init failed";

    H_TEST_ASSERT(r);

    Test909();
    ph.Stop();
    usleep(1000 * 1000); // sleep a while to release the listening address and port
    return 0;
}


