#ifndef RL_EVENT_HPP_
#define RL_EVENT_HPP_

#include <memory>
#include <stdint.h>

struct Event
{
  Event(int64_t time);
  
  virtual ~Event()
  {
  }

  void cancel()
  {
    cancelled_ = true;
  }

  void reschedule(int64_t abs_time)
  {
    reschedule_ = true;
    time_ = abs_time;
  }

  virtual void run() = 0;

  // TODO: make some of these private?
  int64_t time_;
  bool cancelled_;
  bool reschedule_;

  typedef std::shared_ptr<Event> Ptr;
};

void sim_add_event(const Event::Ptr& ev);

int64_t sim_cur_time(void);

void sim_step(void);

#endif // RL_EVENT_HPP_
