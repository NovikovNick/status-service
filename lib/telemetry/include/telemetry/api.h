#ifndef METALHEART_TELEMETRY_API_H
#define METALHEART_TELEMETRY_API_H

#include "counter.h"
#include "gauge.h"
#include "histogram.h"
#include "summary.h"
#include "types.h"

namespace telemetry {

inline CounterBuilder   counter() { return CounterBuilder{}; };
inline GaugeBuilder     gauge() { return GaugeBuilder{}; };
inline HistogramBuilder histogram() { return HistogramBuilder{}; };
inline SummaryBuilder   summary() { return SummaryBuilder{}; };

inline void collect(std::string& out) {
  for (auto [title, metric] : MetricRegistry::instance().metrics_) {
    metric->collect(out);
  }
}

}  // namespace telemetry

#endif  // METALHEART_TELEMETRY_API_H
