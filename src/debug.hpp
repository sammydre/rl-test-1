#pragma once

#include <string>

struct DebugDescription
{
  void set_dbg_desc(const std::string &s) { dbg_desc_ = s; }
  const std::string &get_dbg_desc(void) const { return dbg_desc_; }
private:
  std::string dbg_desc_;
};
