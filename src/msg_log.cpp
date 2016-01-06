#include <cstdarg>

#include "event.hpp"
#include "msg_log.hpp"

struct MsgLog
{
  typedef std::pair<int64_t, std::string> Message;
  typedef std::list<Message> MessageList;


  void add(const std::string &msg)
  {
    add(sim_cur_time(), msg);
  }

  void add(int64_t time, const std::string &msg)
  {
    messages_.push_back(std::make_pair(time, msg));
  }

  const MessageList &get_msg_log() const
  {
    return messages_;
  }

private:
  MessageList messages_;
};


namespace {
  MsgLog g_msg_log;
}

void gmsg(const std::string &msg)
{
  g_msg_log.add(msg);
}

const std::list<std::pair<int64_t, std::string>> &get_msg_log()
{
  return g_msg_log.get_msg_log();
}
