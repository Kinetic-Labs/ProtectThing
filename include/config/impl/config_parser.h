#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "config/config.h"

class ConfigParser final : Config {
public:
    ConfigParser() : Config("ConfigParser", "Parse the main config") {};

    struct RateLimitConfig {
        int requests_per_minute = 100;
        int block_duration_seconds = 60;
    };

    struct PatternProtectionConfig {
        bool enabled = true;
        int path_request_limit = 20;
        int path_time_window_seconds = 10;
    };

    struct ProtectThingConfig {
        std::string name = "default";
        std::string protocol = "http";
        std::string hostname = "localhost";
        std::string portal_bind = "127.0.0.1:9595";
        short port = 9595;
        RateLimitConfig rate_limit;
        PatternProtectionConfig pattern_protection;

        [[nodiscard]] std::string format() const {
            return "Config { Name: " + name +
                   ", Protocol: " + protocol +
                   ", Hostname: " + hostname +
                   ", Portal Bind: " + portal_bind +
                   ", Port: " + std::to_string(port) +
                   ", Rate Limit: { Requests per minute: " + std::to_string(rate_limit.requests_per_minute) +
                   ", Block duration: " + std::to_string(rate_limit.block_duration_seconds) + "s }" +
                   ", Pattern Protection: { Enabled: " + (pattern_protection.enabled ? "true" : "false") +
                   ", Path Request Limit: " + std::to_string(pattern_protection.path_request_limit) +
                   ", Path Time Window: " + std::to_string(pattern_protection.path_time_window_seconds) + "s } }";
        }

        [[nodiscard]] std::string format_url() const {
            return protocol + "://" + hostname + ":" + std::to_string(port) + "/" + name;
        }
    };

    static ProtectThingConfig parse_config(const std::string& path);
};

#endif // CONFIG_PARSER_H
