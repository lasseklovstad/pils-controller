
#include "timeUtils.h"

time_t now;

void configureTime()
{
  // Initialize the time library (you can use NTP or set manually)
  configTime(0, 0, "pool.ntp.org"); // Sync with NTP server

  delay(3000); // Give some time for NTP to sync
}

time_t getTimeStamp()
{
  time(&now);
  return now;
}