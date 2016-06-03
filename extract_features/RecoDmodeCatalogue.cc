#include <algorithm>
#include <cassert>

#include "RecoDmodeCatalogue.h"

const int D0Lund = 421;        
const int DcLund = 411;
const int Dstar0Lund = 423;
const int DstarcLund = 413;
const int KSLund = 310;
const int pi0Lund = 111;
const int KLund = 321;
const int piLund = 211;
const int gammaLund = 22;

RecoDmodeCatalogue::RecoDmodeCatalogue() { RegisterDecays(); }

RecoDmodeCatalogue::~RecoDmodeCatalogue() {};


int RecoDmodeCatalogue::get_d_mode(const std::vector<int> &lund_list) const {
  std::vector<Alphabet> word;
  for (auto l : lund_list) word.push_back(LundToAlphabet(l));
  std::sort(word.begin(), word.end());
  word.push_back(Alphabet::null);
  return static_cast<int>(d_catalogue.find(word));
}


int RecoDmodeCatalogue::get_dstar_mode(const std::vector<int> &lund_list) const {
  std::vector<Alphabet> word;
  for (auto l : lund_list) word.push_back(LundToAlphabet(l));
  std::sort(word.begin(), word.end());
  word.push_back(Alphabet::null);
  return static_cast<int>(dstar_catalogue.find(word));
}


void RecoDmodeCatalogue::RegisterDecays() {

  // D catalogue
  d_catalogue.insert({
      Alphabet::Dc, Alphabet::K, Alphabet::pi, Alphabet::pi, 
      Alphabet::null
    }, Dmode::Dc_Kpipi);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::K, Alphabet::pi, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, Dmode::Dc_Kpipipi0);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::K,
      Alphabet::null
    }, Dmode::Dc_KsK);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::pi,
      Alphabet::null
    }, Dmode::Dc_Kspi);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, Dmode::Dc_Kspipi0);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::Ks, Alphabet::pi, Alphabet::pi, Alphabet::pi,
      Alphabet::null
    }, Dmode::Dc_Kspipipi);

  d_catalogue.insert({
      Alphabet::Dc, Alphabet::K, Alphabet::K, Alphabet::pi,
      Alphabet::null
    }, Dmode::Dc_KKpi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi,
      Alphabet::null
    }, Dmode::D0_Kpi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, Dmode::D0_Kpipi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi, Alphabet::pi, Alphabet::pi,
      Alphabet::null
    }, Dmode::D0_Kpipipi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::pi, Alphabet::pi, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, Dmode::D0_Kpipipipi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::Ks, Alphabet::pi, Alphabet::pi,
      Alphabet::null
    }, Dmode::D0_Kspipi);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::Ks, Alphabet::pi, Alphabet::pi, Alphabet::pi0,
      Alphabet::null
    }, Dmode::D0_Kspipipi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::Ks, Alphabet::pi0,
      Alphabet::null
    }, Dmode::D0_Kspi0);

  d_catalogue.insert({
      Alphabet::D0, Alphabet::K, Alphabet::K,
      Alphabet::null
    }, Dmode::D0_KK);

  // Dstar catalogue
  dstar_catalogue.insert({
      Alphabet::Dstar0, Alphabet::D0, Alphabet::pi0,
      Alphabet::null
    }, Dstarmode::Dstar0_D0pi0);

  dstar_catalogue.insert({
      Alphabet::Dstar0, Alphabet::D0, Alphabet::gamma,
      Alphabet::null
    }, Dstarmode::Dstar0_D0gamma);

  dstar_catalogue.insert({
      Alphabet::Dstarc, Alphabet::D0, Alphabet::pi,
      Alphabet::null
    }, Dstarmode::Dstarc_D0pi);

  dstar_catalogue.insert({
      Alphabet::Dstarc, Alphabet::Dc, Alphabet::pi0,
      Alphabet::null
    }, Dstarmode::Dstarc_Dcpi0);

  dstar_catalogue.insert({
      Alphabet::Dstarc, Alphabet::Dc, Alphabet::gamma,
      Alphabet::null
    }, Dstarmode::Dstarc_Dcgamma);

}

RecoDmodeCatalogue::Alphabet RecoDmodeCatalogue::LundToAlphabet(int lund) const {
  switch (abs(lund)) {
    case DstarcLund:
      return Alphabet::Dstarc;
      break;
    case Dstar0Lund:
      return Alphabet::Dstar0;
      break;
    case DcLund:
      return Alphabet::Dc;
      break;
    case D0Lund:
      return Alphabet::D0;
      break;
    case KLund:
      return Alphabet::K;
      break;
    case KSLund:
      return Alphabet::Ks;
      break;
    case piLund:
      return Alphabet::pi;
      break;
    case pi0Lund:
      return Alphabet::pi0;
      break;
    case gammaLund:
      return Alphabet::gamma;
      break;
    default:
      assert(false);
      return Alphabet::null;
  }
}

