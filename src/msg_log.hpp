#pragma once

#include <cstdint>
#include <list>
#include <string>

void gmsg(const std::string &msg);
void dmsg(const std::string &msg);

typedef std::list<std::pair<int64_t, std::string>> MessageList;

const MessageList &get_msg_log();
const MessageList &get_dbg_log();
