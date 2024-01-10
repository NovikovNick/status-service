#include "telemetry.h"

#include <sys/resource.h>
#include <telemetry/api.h>
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
};  // namespace

namespace m8t {

namespace telemetry {

void collect(std::string& out) {
  auto [rss, cpu_time_total_seconds, uptime_seconds] = getStats();

  ::telemetry::gauge()
      .name("status_service_uptime_in_seconds")  //
      .description("uptime_in_seconds metric")
      .measure(uptime_seconds);
  ::telemetry::gauge()
      .name("status_service_rss_bytes")  //
      .description("current memory consumtions in bytes")
      .measure(rss);
  ::telemetry::gauge()
      .name("status_service_cpu_seconds_total")  //
      .description("Total user and system CPU time spent.")
      .measure(cpu_time_total_seconds);

  ::telemetry::collect(out);
};
}  // namespace telemetry

}  // namespace m8t
