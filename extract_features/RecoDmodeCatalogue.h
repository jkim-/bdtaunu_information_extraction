#include <vector>

#include <trie.h>

class RecoDmodeCatalogue {

  public:

    enum class Dmode { 
      Dc_Kpipi = 1,          
      Dc_Kpipipi0 = 2,      
      Dc_KsK = 3,          
      Dc_Kspi = 4,        
      Dc_Kspipi0 = 5,    
      Dc_Kspipipi = 6,  
      Dc_KKpi = 7,           
      D0_Kpi = 8,           
      D0_Kpipi0 = 9,       
      D0_Kpipipi = 10,    
      D0_Kpipipipi0 = 11,
      D0_Kspipi = 12,        
      D0_Kspipipi0 = 13,    
      D0_Kspi0 = 14,       
      D0_KK = 15,         
      null = -1,         
    };

    enum class Dstarmode {
      NoDstar = 0,                  
      Dstar0_D0pi0 = 1,            
      Dstar0_D0gamma = 2,         
      Dstarc_D0pi = 3,           
      Dstarc_Dcpi0 = 4,         
      Dstarc_Dcgamma = 5,      
      null = -1,              
    };


  public:

    RecoDmodeCatalogue();
    ~RecoDmodeCatalogue();

    // input: vector of lund id's of a D meson followed by its daughters. 
    // output: int converted from Dmode 
    int get_d_mode(const std::vector<int>&) const;

    // input: vector of lund id's of a D* meson followed by its daughters. 
    // output: int converted from Dstarmode 
    int get_dstar_mode(const std::vector<int>&) const;

  private:
    enum class Alphabet { 
      Dstarc, Dstar0, Dc, D0, K, Ks, pi, pi0, gamma, null = -1,
    };

    void RegisterDecays();
    Alphabet LundToAlphabet(int lund) const;

    trie<Alphabet, Dmode, Alphabet::null, Dmode::null> d_catalogue;
    trie<Alphabet, Dstarmode, Alphabet::null, Dstarmode::null> dstar_catalogue;

};
