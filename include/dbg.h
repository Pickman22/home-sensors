#ifndef DBG_H
#define DBG_H

#if defined(__INFO__) || defined(__DBG__) || defined(__WARN__) || defined(__ERR__)
#define DBG_INIT() do { Serial.begin(DBG_BAUD_RATE); Serial.println("\n\r\n\r"); } while(0);
#else
#define DBG_INIT()
#endif

#if defined(__INFO__) || defined(__DBG__) || defined(__WARN__) || defined(__ERR__)
#define DBG_ERR(fmt, ...) printf ("[ERROR] %s::%s(%d) - " fmt "\n\r", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define DBG_ERR(fmt, ...)
#endif

#if defined(__INFO__) || defined(__DBG__) || defined(__WARN__)
#define DBG_WARN(fmt, ...) printf ("[WARN] %s::%s(%d) - " fmt "\n\r", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define DBG_WARN(fmt, ...)
#endif

#if defined(__INFO__) || defined(__DBG__)
#define DBG_INFO(fmt, ...) printf ("[INFO] %s::%s(%d) - " fmt "\n\r", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define DBG_INFO(fmt, ...)
#endif

#ifdef __DBG__
#define DBG_DEBUG(fmt, ...) printf ("[DEBUG] %s::%s(%d) - " fmt "\n\r", __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define DBG_DEBUG(fmt, ...)
#endif

#endif // DBG_H
