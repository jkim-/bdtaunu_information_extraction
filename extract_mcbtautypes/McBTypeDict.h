#ifndef __MCBTYPEDICT_H__
#define __MCBTYPEDICT_H__

#include <vector>
#include <trie.h>

enum class McBTypeCode {
#   define X(a) a,
#   include "McBTypeCode.def"
#   undef X
    null
};

char const* const McBTypeCodeStr[] = {
#   define X(a) #a,
#   include "McBTypeCode.def"
#   undef X
    0
};

inline std::string mcbtype2string(McBTypeCode c) {
  int i = static_cast<int>(c);
  int n = static_cast<int>(McBTypeCode::null);
  return (i >= 0 && i < n) ? std::string(McBTypeCodeStr[i]) : "null";
}

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

class McBTypeDict {

  public:

    McBTypeDict();
    ~McBTypeDict();

    McBTypeCode find(const std::vector<int> &bdaulunds) const;

  private:
    enum class Symbol { 
      nu_ell, nu_tau, ell, tau, 
      D, Dstar, Dstarstar, X, I, null = -1, 
    };

    Symbol LundToSymbol(int lund) const;
    void PopulateDict();

    trie<Symbol, McBTypeCode, Symbol::null, McBTypeCode::null> btype_dict_;

};


#endif
