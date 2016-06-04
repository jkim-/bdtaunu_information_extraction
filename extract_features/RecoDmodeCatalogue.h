#include <vector>

#include <trie.h>

// class that organizes the D decay modes that are used 
// during reconstruction. 
//
// NOTE: modes are indexed according to the sorted list
// of their particle lund id's, after taking absolute values. 
// this means that our indexing does not care about charge 
// conjugation, nor does it care about particle ordering. this is
// usually not a problem if one is only interested in a single mother 
// that decays into a list of daughter particles. 
//
// usage:
//
//     // construct and initialize a device
//     RecoDmodeCatalogue catalogue;
//
//     // given a vector of lund id's, identify the 
//     // corresponding mode index defined in the enum classes below. 
//     std::vector<int> lund_ids = { 411, 321, 211, 211 };
//     catalogue.get_d_mode(lund_ids);
//
// in the example given above for D+ -> K+ pi+ pi-, we needed to provide
// the `lund_ids` consisting of the lund ids of *both* the mother and the 
// daughters. the order in which you provide them is not important. 
class RecoDmodeCatalogue {

  public:

    // index of D decay modes used during reconstruction
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

    // index of D* decay modes used during reconstruction
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

    // constructs and initializes an object ready to accept inputs
    RecoDmodeCatalogue();
    ~RecoDmodeCatalogue();

    // given a vector of lund id's of a D meson and its daughters, 
    // return the mode index that it belongs to. 
    int get_d_mode(const std::vector<int>&) const;

    // given a vector of lund id's of a D* meson and its daughters, 
    // return the mode index that it belongs to. 
    int get_dstar_mode(const std::vector<int>&) const;


  // End of API. Begin implementation
  // --------------------------------
  //
  // Internally, all work is outsourced to a trie search. 

  private:

    // allowed alphabet in the trie's. 
    enum class Alphabet { 
      Dstarc, Dstar0, Dc, D0, K, Ks, pi, pi0, gamma, null = -1,
    };

    // hard code the allowed D decay modes we care to index and 
    // insert them into the tries. 
    void RegisterDecays();

    // maps lund id's to a member in the alphabet
    Alphabet LundToAlphabet(int lund) const;

    // trie's that do the mode identification
    trie<Alphabet, Dmode, Alphabet::null, Dmode::null> d_catalogue;
    trie<Alphabet, Dstarmode, Alphabet::null, Dstarmode::null> dstar_catalogue;

};
