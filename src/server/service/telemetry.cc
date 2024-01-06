#include "telemetry.h"

#include <sys/resource.h>
#include <unistd.h>

#include <chrono>
#include <ctime>
#include <format>
#include <iostream>
#include <unordered_map>

namespace {
struct NodeStats {
  size_t       rss                    = 0;
  double       cpu_time_total_seconds = 0.0;
  unsigned int uptime_seconds         = 0;
};

std::unordered_map<std::string, std::shared_ptr<telemetry::Metric>> metrics_;

NodeStats getStats() {
  NodeStats stats;

  long unsigned utime_ticks        = 0;
  long unsigned stime_ticks        = 0;
  unsigned int  started_time_ticks = 0;
  long unsigned vmsize_bytes       = 0;
  long          rss_pages          = 0;
  double        ticks_per_second   = sysconf(_SC_CLK_TCK);
  long          page_size          = sysconf(_SC_PAGESIZE);
  time_t        now =
      std::chrono::steady_clock::now().time_since_epoch().count() / 1e9;

  FILE* fp = nullptr;
  if ((fp = fopen("/proc/self/stat", "r")) == nullptr) {
    return stats;
  }

  if (fscanf(fp,
             "%*d "  //  1. PID
             "%*s "  //  2. Executable
             "%*c "  //  3. State
             "%*d "  //  4. Parent PID
             "%*d "  //  5. Process group ID
             "%*d "  //  6. Session ID
             "%*d "  //  7. Controlling terminal
             "%*d "  //  8. Foreground process group ID
             "%*u "  //  9. Flags
             "%*u "  // 10. Number of minor faults
             "%*u "  // 11. Number of minor faults of waited-for children
             "%*u "  // 12. Number of major faults
             "%*u "  // 13. Number of major faults of waited-for children
             "%lu "  // 14. CPU user time in ticks
             "%lu "  // 15. CPU kernel time in ticks
             "%*d "  // 16. CPU user time of waited-for children
             "%*d "  // 17. CPU kernel time of waited-for children
             "%*d "  // 18. Priority
             "%*d "  // 19. Nice value
             "%*d "  // 20. Num threads
             "%*d "  // 21. Obsolete since 2.6
             "%u "   // 22. Time the process started after system boot
             "%lu "  // 23. Virtual memory size in bytes
             "%ld",  // 24. Resident set size in pages
             &utime_ticks,
             &stime_ticks,
             &started_time_ticks,
             &vmsize_bytes,
             &rss_pages) != 5) {
    fclose(fp);
    return stats; /* Can't read? */
  }
  fclose(fp);

  stats.rss                    = rss_pages * page_size;
  stats.cpu_time_total_seconds = (utime_ticks + stime_ticks) / ticks_per_second;
  stats.uptime_seconds         = now - (started_time_ticks / ticks_per_second);

  return stats;
}

template <typename T, typename... Args>
T& getMetricOrCreateIfNotExist(std::string_view title,
                               std::string_view description,
                               Args&&... args) {
  std::string key{title};
  if (auto it = metrics_.find(key); it != metrics_.end()) {
    auto& metric = *(it->second.get());
    return static_cast<T&>(metric);
  } else {
    auto metric   = std::make_shared<T>(title,  //
                                      description,
                                      std::forward<Args>(args)...);
    metrics_[key] = metric;
    return static_cast<T&>(*(metric.get()));
  }
}
};  // namespace

namespace m8t {

namespace telemetry {

::telemetry::Gauge& gauge(std::string_view title,
                          std::string_view description) {
  return getMetricOrCreateIfNotExist<::telemetry::Gauge>(title, description);
}

template <typename... Q>
::telemetry::Summary& summary(std::string_view          title,
                              std::string_view          description,
                              std::chrono::milliseconds observation_time,
                              Q&&... quantiles) {
  return getMetricOrCreateIfNotExist<::telemetry::Summary>(
      title,  //
      description,
      observation_time,
      std::forward<Q>(quantiles)...);
}

void collect(std::string& out) {
  auto [rss, cpu_time_total_seconds, uptime_seconds] = getStats();

  gauge("status_service_uptime_in_seconds",  //
        "uptime_in_seconds metric")
      .set(uptime_seconds);

  gauge("status_service_rss_bytes",  //
        "current memory consumtions in bytes")
      .set(rss);

  gauge("status_service_cpu_seconds_total",
        "Total user and system CPU time spent.")
      .set(cpu_time_total_seconds);

  for (auto [title, metric] : metrics_) {
    metric->collect(out);
  }
};
}  // namespace telemetry

template ::telemetry::Summary& telemetry::summary(std::string_view,  //
                                                  std::string_view,
                                                  std::chrono::milliseconds,
                                                  double&&,
                                                  double&&,
                                                  double&&,
                                                  double&&);

}  // namespace m8t
