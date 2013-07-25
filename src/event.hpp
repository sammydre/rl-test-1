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

  bool operator<(const Event &rhs) const
  {
    // We reverse the comparison here to make the priority queue have
    // the lowest element at the top, not the greatest.
    return time_ > rhs.time_;
  }

  void cancel()
  {
    cancelled_ = true;
  }

  virtual void run() = 0;

  int64_t time_;
  bool cancelled_;
  bool reschedule_;

  typedef std::shared_ptr<Event> Ptr;
};

void sim_add_event(const Event::Ptr& ev);

int64_t sim_cur_time(void);

void sim_step(void);

#endif // RL_EVENT_HPP_
