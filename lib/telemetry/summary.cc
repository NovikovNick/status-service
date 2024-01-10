#include "summary.h"

#include <cmath>
#include <format>

namespace telemetry {

void Summary::observe(double value) {
  Timepoint now     = Clock::now();
  Timepoint expired = now - observation_time_;

  auto lk = std::unique_lock(mutex_);
  auto it = values_.insert(value);
  refs_.push(std::make_pair(now, it));

  while (!refs_.empty() && refs_.front().first < expired) {
    values_.erase(refs_.front().second);
    refs_.pop();
  }
};

void Summary::collect(std::string& out) {
  // usage of vector here not optimal, but more clearly to read
  std::vector<double> tmp_vector;
  size_t              count = 0;
  double              sum   = 0;

  {
    auto lk = std::unique_lock(mutex_);
    count   = values_.size();
    tmp_vector.resize(count);
    int i = 0;
    for (auto val : values_) {
      tmp_vector[i] = val;
      sum += val;
      ++i;
    }
  }

  out += std::format("# HELP {} {}\n", title_, description_);
  out += std::format("# TYPE {} summary\n", title_);

  for (auto quantile : quantiles_) {
    double index      = (count - 1) * quantile;
    size_t index_low  = std::floor(index);
    size_t index_high = std::ceil(index);
    double value      = 0;

    if (count == 0) {
      value = 0;
    } else if (index_low == index_high) {
      value = tmp_vector[index_low];
    } else {
      auto d0 = tmp_vector[index_low] * (index_high - index);
      auto d1 = tmp_vector[index_high] * (index - index_low);
      value   = d0 + d1;
    }

    out += std::format("{}{{quantile=\"{}\"}} {}\n",  //
                       title_,
                       quantile,
                       value);
  }

  out += std::format("{}_sum {}\n", title_, sum);
  out += std::format("{}_count {}\n", title_, count);
};

}  // namespace telemetry
