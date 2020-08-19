#ifndef NEU_H
#define NEU_H

#include <stdio.h>

#define NEU_DEBUG 1

//******************************************************************** Debug{{{
#ifdef _WIN32
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#if NEU_DEBUG
#define NeuInfo(fmt, ...) printf("[%s] " fmt "\n", __FILENAME__, ##__VA_ARGS__)
#define NeuErr(fmt, ...) printf("[%s > %s > %d] " fmt "\n", __FILENAME__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#define NeuOL(fmt, ...) printf("\r[%s] " fmt, __FILENAME__, ##__VA_ARGS__); fflush(stdout); // Print on one line.
#else
#define NeuInfo(fmt, ...)
#define NeuErr(fmt, ...)
#define NeuOL(fmt, ...)
#endif
//******************************************************************** Debug}}}

typedef enum
{
  NEU_OK   = 0x00000000,
  NEU_FAIL = 0x80000000,
} NRESULT;

#define NeuSucc(nr) (((NRESULT)(nr)) >= 0)
#define NeuFail(nr) (((NRESULT)(nr)) < 0)

#define NeuRelease(o) \
  if ((o) != nullptr) \
  { \
    delete (o); \
    (o) = nullptr; \
  }

#endif  // NEU_H
