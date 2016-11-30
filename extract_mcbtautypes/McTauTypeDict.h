#ifndef __MCTAUTYPEDICT_H__
#define __MCTAUTYPEDICT_H__

enum class McTauTypeCode {
#   define X(a) a,
#   include "McTauTypeCode.def"
#   undef X
    null
};

char const* const McTauTypeCodeStr[] = {
#   define X(a) #a,
#   include "McTauTypeCode.def"
#   undef X
    0
};

inline std::string mctautype2string(McTauTypeCode c) {
  int i = static_cast<int>(c);
  int n = static_cast<int>(McTauTypeCode::null);
  return (i >= 0 && i < n) ? std::string(McTauTypeCodeStr[i]) : "null";
}

class McTauTypeDict {

  public:

    McTauTypeDict();
    ~McTauTypeDict();

};


#endif
