#include "FT1060Mexcept.hpp"
FT1060MException::FT1060MException (const char *msg) : message (msg) {}
FT1060MException::FT1060MException (const std::string msg) : message (msg) {}
const char *FT1060MException::what () const noexcept {
  return message.c_str ();
}