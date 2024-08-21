
#ifndef RF_JSON_H
#define RF_JSON_H

#include "nlohmann/json.hpp"

namespace rfbase
{
using RfJson = nlohmann::json;
using RfJsonArray = nlohmann::json::array_t;
class JsonUtils
{
public:
    [[nodiscard]] static bool loadJson(const std::string &path, nlohmann::json &json);
    [[nodiscard]] static bool saveJson(const nlohmann::json &obj, const std::string &path);

    static inline std::string toJsonString(const RfJson &json)
    {
        return json.dump();
    }
    static inline RfJson fromJsonString(const std::string &jsonStr)
    {
        return RfJson::parse(jsonStr);
    }
    static inline int getInt(const RfJson &json, const std::string &key)
    {
        return json.at(key).get<int>();
    }
    static inline unsigned int getUInt(const RfJson &json, const std::string &key)
    {
        return json.at(key).get<unsigned int>();
    }
    static inline std::string getString(const RfJson &json, const std::string &key)
    {
        return json.at(key).get<std::string>();
    }
    static inline bool getBool(const RfJson &json, const std::string &key)
    {
        return json.at(key).get<bool>();
    }
    static inline double getDouble(const RfJson &json, const std::string &key)
    {
        return json.at(key).get<double>();
    }
    static inline RfJson getJson(const RfJson &json, const std::string &key)
    {
        return json.at(key);
    }
    static inline RfJsonArray getJsonArray(const RfJson &json, const std::string &key)
    {
        return json.at(key).get<RfJsonArray>();
    }

    template <typename T>
    static void getArrayValue(const RfJson &json, const std::string &key, std::vector<T> &vec)
    {
        const RfJsonArray &arr = json.at(key).get<RfJsonArray>();
        for (const auto &item : arr)
        {
            vec.push_back(item.get<T>());
        }
    }
};
} // namespace rfbase
#endif
