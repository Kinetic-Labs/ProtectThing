#include "config/impl/config_parser.h"

#include <cJSON.h>
#include "util/impl/fs.h"
#include "util/impl/logger.h"

ConfigParser::ProtectThingConfig ConfigParser::parse_config(const std::string& path) {
    ProtectThingConfig config;

    const std::string json_string = FS::read_to_string(path);
    cJSON* p_json = cJSON_Parse(json_string.c_str());

    if(!p_json) {
        const char* p_error = cJSON_GetErrorPtr();
        std::string error_msg = "JSON Parse Error";

        if(p_error)
            error_msg += ": " + std::string(p_error);

        throw std::runtime_error(error_msg);
    }

    try {
        const cJSON* p_name = cJSON_GetObjectItemCaseSensitive(p_json, "name");
        const cJSON* p_protocol = cJSON_GetObjectItemCaseSensitive(p_json, "protocol");
        const cJSON* p_hostname = cJSON_GetObjectItemCaseSensitive(p_json, "hostname");
        const cJSON* p_portal_bind = cJSON_GetObjectItemCaseSensitive(p_json, "portal:bind");
        const cJSON* p_port = cJSON_GetObjectItemCaseSensitive(p_json, "port");
        const std::string name_str = p_name->valuestring;
        const std::string protocol_str = p_protocol->valuestring;
        const std::string hostname_str = p_hostname->valuestring;
        const std::string portal_bind_str = p_portal_bind->valuestring;
        const auto port_short = static_cast<short>(p_port->valueint);

        config.name = name_str;
        config.protocol = protocol_str;
        config.hostname = hostname_str;
        config.portal_bind = portal_bind_str;
        config.port = port_short;

        const cJSON* p_rate_limit = cJSON_GetObjectItemCaseSensitive(p_json, "rate_limit");
        if (p_rate_limit) {
            const cJSON* p_requests_per_minute = cJSON_GetObjectItemCaseSensitive(p_rate_limit, "requests_per_minute");
            const cJSON* p_block_duration_seconds = cJSON_GetObjectItemCaseSensitive(p_rate_limit, "block_duration_seconds");

            if (p_requests_per_minute) {
                config.rate_limit.requests_per_minute = p_requests_per_minute->valueint;
            }
            if (p_block_duration_seconds) {
                config.rate_limit.block_duration_seconds = p_block_duration_seconds->valueint;
            }
        }

        const cJSON* p_pattern_protection = cJSON_GetObjectItemCaseSensitive(p_json, "pattern_protection");
        if (p_pattern_protection) {
            const cJSON* p_enabled = cJSON_GetObjectItemCaseSensitive(p_pattern_protection, "enabled");
            const cJSON* p_path_request_limit = cJSON_GetObjectItemCaseSensitive(p_pattern_protection, "path_request_limit");
            const cJSON* p_path_time_window_seconds = cJSON_GetObjectItemCaseSensitive(p_pattern_protection, "path_time_window_seconds");

            if (p_enabled) {
                config.pattern_protection.enabled = cJSON_IsTrue(p_enabled);
            }
            if (p_path_request_limit) {
                config.pattern_protection.path_request_limit = p_path_request_limit->valueint;
            }
            if (p_path_time_window_seconds) {
                config.pattern_protection.path_time_window_seconds = p_path_time_window_seconds->valueint;
            }
        }
    } catch(const std::exception &exception) {
        Logger::error(exception.what(), "ConfigParser");
        cJSON_Delete(p_json);

        throw;
    }

    cJSON_Delete(p_json);
    return config;
}

