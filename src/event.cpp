#include <queue>
#include <stdio.h>
#include "event.hpp"

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

    std::shared_ptr<Event> ev = queue_.top();
    queue_.pop();

    if (!ev->cancelled_) {
      fprintf(stderr, "Sim: run event at %ld\n", ev->time_);
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

  int64_t time_;
  std::priority_queue<std::shared_ptr<Event>> queue_;
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

