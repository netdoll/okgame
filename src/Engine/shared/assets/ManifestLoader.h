#pragma once
#include <string>
#include <vector>
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include <iostream>

namespace OKGame {
    class ManifestLoader {
    public:
        void load(const std::string& host, int port, const std::string& path) {
            try {
                Poco::Net::HTTPClientSession session(host, port);
                Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, path, Poco::Net::HTTPMessage::HTTP_1_1);
                session.sendRequest(request);

                Poco::Net::HTTPResponse response;
                std::istream& rs = session.receiveResponse(response);

                if (response.getStatus() == Poco::Net::HTTPResponse::HTTP_OK) {
                    Poco::JSON::Parser parser;
                    Poco::Dynamic::Var result = parser.parse(rs);
                    Poco::JSON::Object::Ptr obj = result.extract<Poco::JSON::Object::Ptr>();
                    
                    std::cout << "[ManifestLoader] Loading manifest v" << obj->getValue<std::string>("version") << std::endl;
                    // Logic to iterate over assets and trigger native loads
                }
            } catch (const std::exception& e) {
                std::cerr << "[ManifestLoader] Error: " << e.what() << std::endl;
            }
        }
    };
}
