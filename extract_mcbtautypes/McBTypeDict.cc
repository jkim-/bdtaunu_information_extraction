#include "McBTypeDict.h"
#include "extract_mcbtypes_constants.h"

McBTypeDict::McBTypeDict() { PopulateDict(); }
McBTypeDict::~McBTypeDict() {}

McBTypeCode McBTypeDict::find(const std::vector<int> &bdaulunds) const {

  std::vector<Symbol> word;
  bool hasX = false;
  for (auto l : bdaulunds) {
    Symbol a = LundToSymbol(l);
    if (a == Symbol::I) {
      continue;
    } else if (a == Symbol::X) {
      hasX = true;
    } else {
      word.push_back(a);
    }
  }

  std::sort(word.begin(), word.end());
  if (hasX) word.push_back(Symbol::X);
  word.push_back(Symbol::null);

  return btype_dict_.find(word);
}

void McBTypeDict::PopulateDict() {

  // nu_tau branch
  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, 
      Symbol::null
    }, McBTypeCode::SL);

  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, Symbol::D,
      Symbol::null
    }, McBTypeCode::Dtau);

  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, Symbol::Dstar,
      Symbol::null
    }, McBTypeCode::Dstartau);

  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, Symbol::Dstarstar,
      Symbol::null
    }, McBTypeCode::Dstarstar_res);

  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, Symbol::X,
      Symbol::null
    }, McBTypeCode::SL);

  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, 
      Symbol::D, Symbol::X,
      Symbol::null
    }, McBTypeCode::Dstarstar_nonres);

  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, 
      Symbol::Dstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Dstarstar_nonres);

  btype_dict_.insert({
      Symbol::nu_tau, Symbol::tau, 
      Symbol::Dstarstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Dstarstar_res);

  // nu_ell branch
  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, 
      Symbol::null
    }, McBTypeCode::SL);

  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, Symbol::D,
      Symbol::null
    }, McBTypeCode::Dl);

  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, Symbol::Dstar,
      Symbol::null
    }, McBTypeCode::Dstarl);

  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, Symbol::Dstarstar,
      Symbol::null
    }, McBTypeCode::Dstarstar_res);

  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, Symbol::X,
      Symbol::null
    }, McBTypeCode::SL);

  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, 
      Symbol::D, Symbol::X,
      Symbol::null
    }, McBTypeCode::Dstarstar_nonres);

  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, 
      Symbol::Dstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Dstarstar_nonres);

  btype_dict_.insert({
      Symbol::nu_ell, Symbol::ell, 
      Symbol::Dstarstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Dstarstar_res);

  // hadron branch
  btype_dict_.insert({
      Symbol::X,
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::D, Symbol::X, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstar, Symbol::X, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstarstar, Symbol::X, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::D, Symbol::D, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::D, Symbol::Dstar, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::D, Symbol::Dstarstar, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstar, Symbol::Dstar, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstar, Symbol::Dstarstar, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstarstar, Symbol::Dstarstar, 
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::D, Symbol::D, Symbol::X,
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::D, Symbol::Dstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::D, Symbol::Dstarstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstar, Symbol::Dstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstar, Symbol::Dstarstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Had);

  btype_dict_.insert({
      Symbol::Dstarstar, Symbol::Dstarstar, Symbol::X,
      Symbol::null
    }, McBTypeCode::Had);

}

McBTypeDict::Symbol McBTypeDict::LundToSymbol(int lund) const {
  switch (std::abs(lund)) {
    case nueLund: 
    case numuLund: 
      return Symbol::nu_ell;
      break;
    case nutauLund:
      return Symbol::nu_tau;
      break;
    case eLund: 
    case muLund:
      return Symbol::ell;
      break;
    case tauLund:
      return Symbol::tau;
      break;
    case DpLund:
    case D0Lund:
      return Symbol::D;
      break;
    case DstarpLund: 
    case Dstar0Lund:
      return Symbol::Dstar;
      break;
    case D1pLund:
    case D10Lund:
    case D2pLund:
    case D20Lund:
    case D1primepLund:
    case D1prime0Lund:
    case D0sspLund:
    case D0ss0Lund:
    case D2SpLund:
    case D2S0Lund:
    case D2SstarpLund:
    case D2Sstar0Lund:
      return Symbol::Dstarstar;
      break;
    case gammaLund: 
      return Symbol::I;
      break;
    default: 
      return Symbol::X;
      break;
  }
}
