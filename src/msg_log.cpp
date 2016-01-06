#include <cstdarg>

#include "event.hpp"
#include "msg_log.hpp"

struct MsgLog
{
  void add(const std::string &msg)
  {
    add(sim_cur_time(), msg);
  }
  void add_dbg(const std::string &msg)
  {
    add_dbg(sim_cur_time(), msg);
  }
  void add(int64_t time, const std::string &msg)
  {
    messages_.push_back(std::make_pair(time, msg));
  }
  void add_dbg(int64_t time, const std::string &msg)
  {
    debug_messages_.push_back(std::make_pair(time, msg));
  }
  const MessageList &get_msg_log() const
  {
    return messages_;
  }
  const MessageList &get_dbg_log() const
  {
    return debug_messages_;
  };

private:
  MessageList messages_;
  MessageList debug_messages_;
};


namespace {
  MsgLog g_msg_log;
}

void gmsg(const std::string &msg)
{
  g_msg_log.add(msg);
}

void dmsg(const std::string &msg)
{
  g_msg_log.add_dbg(msg);
}

const MessageList &get_msg_log()
{
  return g_msg_log.get_msg_log();
}

const MessageList &get_dbg_log()
{
  return g_msg_log.get_dbg_log();
}
