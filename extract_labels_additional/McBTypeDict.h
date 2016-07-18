#ifndef __MCBTYPEDICT_H__
#define __MCBTYPEDICT_H__

#include <vector>
#include <trie.h>

enum class McBTypeCode { 
  NoB = 0,
  Dtau,
  Dstartau,
  Dl,
  Dstarl, 
  Dstarstar_res,
  Dstarstar_nonres,
  SL,
  Had,
  null
};

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
