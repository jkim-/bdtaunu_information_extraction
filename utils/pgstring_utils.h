#ifndef _PGSTRING_CONVERT_H_
#define _PGSTRING_CONVERT_H_

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <boost/tokenizer.hpp>

// helper classes and objects
// --------------------------

// trait classes and specializations
template <typename T>
class pgstring_conversion_traits;

template <>
class pgstring_conversion_traits<int> {
  public:

    static int convert_string2type(const std::string &s) { 
      return std::stoi(s); 
    }

    static std::string convert_type2string(const int &v) { 
      return std::to_string(v); 
    }
};

template <>
class pgstring_conversion_traits<float> {
  public:

    static float convert_string2type(const std::string &s) { 
      return std::stof(s); 
    }

    static std::string convert_type2string(const float &v) { 
      pgstring_convert_float_ss_.str("");
      pgstring_convert_float_ss_.clear();
      pgstring_convert_float_ss_ << v;
      return pgstring_convert_float_ss_.str();
    }

  private:
    static std::stringstream pgstring_convert_float_ss_;
    static std::stringstream initialize_sstream();
};

std::stringstream 
pgstring_conversion_traits<float>::initialize_sstream() {
  std::stringstream ss; ss.precision(10);
  return ss;
}

std::stringstream 
pgstring_conversion_traits<float>::pgstring_convert_float_ss_ 
  = pgstring_conversion_traits<float>::initialize_sstream();


template <>
class pgstring_conversion_traits<double> {
  public:
    static double convert_string2type(const std::string &s) { 
      return std::stod(s); 
    }

    static std::string convert_type2string(const double &v) { 
      pgstring_convert_double_ss_.str("");
      pgstring_convert_double_ss_.clear();
      pgstring_convert_double_ss_ << v;
      return pgstring_convert_double_ss_.str();
    }

  private:
    static std::stringstream pgstring_convert_double_ss_;
    static std::stringstream initialize_sstream();
};

std::stringstream 
pgstring_conversion_traits<double>::initialize_sstream() {
  std::stringstream ss; ss.precision(19);
  return ss;
}

std::stringstream 
pgstring_conversion_traits<double>::pgstring_convert_double_ss_ 
  = pgstring_conversion_traits<double>::initialize_sstream();


// fuctions converting text => binary type
// ---------------------------------------

// functions that convert postgres text data to fundamental types 
inline void pgstring_convert(const std::string &s, int &v) { 
  v = pgstring_conversion_traits<int>::convert_string2type(s); 
}

inline void pgstring_convert(const std::string &s, float &v) { 
  v = pgstring_conversion_traits<float>::convert_string2type(s); 
}

inline void pgstring_convert(const std::string &s, double &v) { 
  v = pgstring_conversion_traits<double>::convert_string2type(s); 
}

// function that convert postgres text data to std::vector
template <typename T> 
void pgstring_convert(const std::string &s, std::vector<T> &v, 
    const std::string &enclosure_chars="{}",
    const boost::char_separator<char> &sep = 
    boost::char_separator<char>(",", "", boost::keep_empty_tokens)) {

  v.clear();

  size_t first_idx = s.find_first_not_of(enclosure_chars);
  size_t last_idx = s.find_last_not_of(enclosure_chars);
  if (first_idx == std::string::npos || last_idx == std::string::npos) { return; }

  boost::tokenizer<boost::char_separator<char>> tok(
      s.begin()+first_idx, s.begin()+last_idx+1, sep);

  transform(tok.begin(), tok.end(), 
            std::back_inserter(v), 
            pgstring_conversion_traits<T>::convert_string2type);
}

// fuctions converting binary type => text (with controlled precision)
// -------------------------------------------------------------------

template <typename T> 
inline std::string pgstring_convert_to_string(const T &v) {
  return pgstring_conversion_traits<T>::convert_type2string(v);
}

// function that converts std::vector to postgres text data. 
template <typename T> 
std::string vector2pgstring(const std::vector<T> &v) {

  if (v.empty()) { return "{}"; }

  std::string s = "\"{";

  for (const auto &e : v) {
    s += pgstring_convert_to_string(e) + ",";
  }

  s.pop_back(); s += "}\"";
  return s;
}

#endif
