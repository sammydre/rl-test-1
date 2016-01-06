#pragma once

#include <cstdint>
#include <list>
#include <string>

void gmsg(const std::string &msg);

const std::list<std::pair<int64_t, std::string>> &get_msg_log();
