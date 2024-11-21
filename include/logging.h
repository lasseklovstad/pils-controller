#ifndef LOGGING_H
#define LOGGING_H

// Define logging levels
#define LOG_LEVEL_DEBUG 2
#define LOG_LEVEL_ERROR 1
#define LOG_LEVEL_OFF 0

// Set the active logging level (can be adjusted for development/production)
#define CURRENT_LOG_LEVEL LOG_LEVEL_OFF

// ANSI escape codes for color
#define COLOR_RESET   "\033[0m"
#define COLOR_DEBUG   "\033[34m"  // Blue
#define COLOR_ERROR   "\033[31m"  // Red

// Conditional logging macros
#if CURRENT_LOG_LEVEL >= LOG_LEVEL_DEBUG
  #define LOG_DEBUG(msg) Serial.println(COLOR_DEBUG "[DEBUG] " + String(msg) + COLOR_RESET)
#else
  #define LOG_DEBUG(msg)
#endif

#if CURRENT_LOG_LEVEL >= LOG_LEVEL_ERROR
  #define LOG_ERROR(msg) Serial.println(COLOR_ERROR "[ERROR] " + String(msg) + COLOR_RESET)
#else
  #define LOG_ERROR(msg)
#endif

#endif // LOGGING_H
