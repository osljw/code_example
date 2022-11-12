#include <string>
#include <iostream>

#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"


#define PARSE_JSON_MEMBER_STRING(_adidea_, _json_doc_, _member_name_)  \
    _adidea_._member_name_ = _json_doc_[#_member_name_].IsString()             \
        ? _json_doc_[#_member_name_].GetString() : "";

#define CHECK_JSON_MEMBER_EXIST(json_doc, _member_name_)  \
    if (!json_doc.HasMember(#_member_name_)) {   \
        std::cout << "no json" << #_member_name_ << std::endl;  \
        return -1;                            \
    }

std::string json_str = "[{\"name\":\"hello\", \"value\":\"你好\"}]";
//std::string json_str = "{name:hello}";

int main() {
    rapidjson::Document json_doc;
    json_doc.Parse(json_str.c_str());
    if (json_doc.HasParseError()) {
        //RUN_LOG_ERROR("parse adidea json error");
        std::cout << "parse error" << std::endl;
        return -1;
    }

    if (json_doc.IsArray()) {
        std::cout << "json_doc is Array" << std::endl;

        // json Array loop, use ConstValueIterator
        for (rapidjson::Value::ConstValueIterator itr = json_doc.Begin(); itr != json_doc.End(); ++itr) {
            const rapidjson::Value& attribute = *itr;

            if(attribute.IsObject() && attribute.HasMember("name")) {
                // json object to string
                rapidjson::StringBuffer sbBuf;
                rapidjson::Writer<rapidjson::StringBuffer> jWriter(sbBuf);
                attribute.Accept(jWriter);
                std::cout << "json object tostring: " << std::string(sbBuf.GetString()) << std::endl;

                // json Object loop, use ConstMemberIterator
                for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
                    std::cout << itr2->name.GetString() << " : " << itr2->value.GetString() << std::endl;
                }
            }
        }
    }

    return 0;
}
