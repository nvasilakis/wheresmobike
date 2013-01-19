#pragma once

#include <cstdlib>
#include <iostream>

namespace wmb
{

#define MSG_BASE(stream, body) \
  do { try { \
    stream << __FILE__ << ":" << __FUNCTION__ << "():" << __LINE__ << ": " \
            << body << ::std::endl; \
  } catch (...) { /* don't throw anything while trying to output messages*/ } \
  } while(0)

// DEBUG_STR(body) is conditionally defined
#define INFO_STR(body) MSG_BASE(::std::cout, "INFO: " << body)
#define WARN_STR(body) MSG_BASE(::std::cerr, "WARN: " << body)
#define ERROR_STR(body) MSG_BASE(::std::cerr, "ERROR: " << body)
#define FATAL_STR(body) MSG_BASE(::std::cerr, "FATAL: " << body)


#if defined(NDEBUG) || defined(NDEBUG_LOG)
#define DEBUG_STR(body)
#else
#define DEBUG_STR(body) MSG_BASE(::std::clog, "DEBUG: " << body)
#endif

#define DEBUG(var) DEBUG_STR(#var ": " << var)
#define INFO(var) INFO_STR(#var ": " << var)
#define WARN(var) WARN_STR(#var ": " << var)
#define ERROR(var) ERROR_STR(#var ": " << var)
#define FATAL(var) FATAL_STR(#var ": " << var)

} // namespace wmb
