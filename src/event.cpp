#include <queue>
#include <cstdio>
#include <inttypes.h>

#include "event.hpp"
#include "msg_log.hpp"
#include "tinyformat.h"

Event::Event(int64_t time)
  : time_(time),
    cancelled_(false),
    reschedule_(false)
{
}

namespace {

struct Simulator
{
  Simulator()
    : time_(0)
  {
  }

  void step()
  {
    if (queue_.empty())
      return;

    auto ev = queue_.top();
    queue_.pop();

    if (!ev->cancelled_) {
      gmsg(tfm::format("Sim: t=%d, event=%s", ev->time_, ev->get_dbg_desc()));
      // gui_msg("Sim: run event at %" PRIi64 "\n", ev->time_);
      time_ = ev->time_;
      ev->run();
    }

    if (ev->reschedule_) {
      ev->cancelled_ = false;
      ev->reschedule_ = false;
      add(ev);
    }
  }

  void add(const std::shared_ptr<Event>& ev)
  {
    queue_.push(ev);
  }

  struct EventComparitor
  {
    bool operator()(const Event::Ptr &lhs, const Event::Ptr &rhs) const
    {
      // We reverse the comparison here to make the priority queue have
      // the lowest element at the top, not the greatest.
      return lhs->time_ > rhs->time_;
    }
  };

  int64_t time_;
  std::priority_queue<Event::Ptr,
                      std::vector<Event::Ptr>,
                      EventComparitor> queue_;
};

Simulator sim;
}

void sim_add_event(const std::shared_ptr<Event>& ev)
{
  sim.add(ev);
}

int64_t sim_cur_time(void)
{
  return sim.time_;
}

void sim_step(void)
{
  sim.step();
}

int sim_num_events(void)
{
  return sim.queue_.size();
}

