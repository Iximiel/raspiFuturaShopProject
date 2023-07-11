#ifndef FT1060EXCEPT_HPP
#define FT1060EXCEPT_HPP
#include <exception>
#include <string>
struct FT1060MException : public std::exception {
  FT1060MException (const char *msg);
  FT1060MException (const std::string msg);
  const char *what () const noexcept;

private:
   std::string message;
};

#endif // FT1060EXCEPT_HPP
