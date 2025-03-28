#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>

#include "evpp/functional.h"
#include "evpp/httpc/url_parser.h"

namespace evpp {
namespace httpc {
using namespace std;
static const std::string default_http_port = "80";
static bool equal_key(char v) {
    return v == ':' || v == '/' || v == '?' || v == '#';
}

static bool is_question_or_sharp(char v) {
    return v == '?' || v == '#';
}

URLParser::URLParser(const std::string& url) : port(80) {
    parse(url);
}

int URLParser::parse(const string& url_s) {
    string::const_iterator it;
    string::const_iterator last_it = url_s.begin();

    static const string prot_end("://");
    it = search(url_s.begin(), url_s.end(), prot_end.begin(), prot_end.end());
    if (it != url_s.end()) {
        schema.reserve(distance(url_s.begin(), it));
        std::transform(url_s.begin(), it, std::back_inserter(schema), [](unsigned char c) { return std::tolower(c); });
        advance(it, prot_end.length());
        last_it = it;
    }

    it = find_if(last_it, url_s.end(), equal_key);

    host.reserve(distance(last_it, it));
    std::transform(last_it, it, std::back_inserter(host), [](unsigned char c) { return std::tolower(c); });

    if (it == url_s.end()) {
        return 0;
    }

    if (*it == ':') {
        it++;

        if (it != url_s.end()) {
            last_it = it;
            it = find_if(last_it, url_s.end(), equal_key);
            port = ::atoi(&(*last_it));
        }
    }

    if (it != url_s.end() && *it == '/') {
        last_it = it;
        it = find_if(last_it, url_s.end(), is_question_or_sharp);
        path.assign(last_it, it);
    }

    if (it != url_s.end() && *it == '?') {
        it++;

        if (it != url_s.end()) {
            last_it = it;
            query.assign(last_it, url_s.end());
        }
    }

    return 0;
}
}
}
