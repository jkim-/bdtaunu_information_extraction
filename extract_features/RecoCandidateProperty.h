#ifndef __RECOCANDIDATEPROPERTY_H__
#define __RECOCANDIDATEPROPERTY_H__

class RecoCandidateProperty {
  public:
    virtual ~RecoCandidateProperty() = 0;
};

inline RecoCandidateProperty::~RecoCandidateProperty() {}

class gammaCandidateProperty : public RecoCandidateProperty {
  public:
    gammaCandidateProperty() {}
    ~gammaCandidateProperty() {}
};

class lCandidateProperty : public RecoCandidateProperty {
  public:
    lCandidateProperty() : e_pid_(), mu_pid_() {}
    ~lCandidateProperty() {}

    int get_e_pid() const { return e_pid_; }
    void set_e_pid(int e_pid) { e_pid_ = e_pid; }

    int get_mu_pid() const { return mu_pid_; }
    void set_mu_pid(int mu_pid) { mu_pid_ = mu_pid; }

  private:
    int e_pid_;
    int mu_pid_;
};

class hCandidateProperty : public RecoCandidateProperty {
  public:
    hCandidateProperty() : e_pid_(), mu_pid_() {}
    ~hCandidateProperty() {}

    int get_e_pid() const { return e_pid_; }
    void set_e_pid(int e_pid) { e_pid_ = e_pid; }

    int get_mu_pid() const { return mu_pid_; }
    void set_mu_pid(int mu_pid) { mu_pid_ = mu_pid; }

  private:
    int e_pid_;
    int mu_pid_;
};

class CCandidateProperty : public RecoCandidateProperty {
  public:
    CCandidateProperty() {}
    ~CCandidateProperty() {}
};

class DCandidateProperty : public RecoCandidateProperty {
  public:
    DCandidateProperty() {}
    ~DCandidateProperty() {}

    bool is_dstar() const { return is_dstar_; }
    void set_is_dstar(bool is_dstar) { is_dstar_ = is_dstar; }

    float get_dmass() const { return dmass_; }
    void set_dmass(float dmass) { dmass_ = dmass; }

    int get_d_mode() const { return d_mode_; }
    void set_d_mode(int d_mode) { d_mode_ = d_mode; }

    int get_dstar_mode() const { return dstar_mode_; }
    void set_dstar_mode(int dstar_mode) { dstar_mode_ = dstar_mode; }

  private:
    bool is_dstar_;

    int d_mode_;
    int dstar_mode_;

    float dmass_;
};

class BCandidateProperty : public RecoCandidateProperty {
  public:
    BCandidateProperty() {}
    ~BCandidateProperty() {}

    bool is_tag_b() const { return is_tag_b_; }
    void set_is_tag_b(bool is_tag_b) { is_tag_b_ = is_tag_b; }

    void set_d_daughter(DCandidateProperty *d) { d_ = d; }
    void set_h_daughter(hCandidateProperty *h) { h_ = h; }
    void set_l_daughter(lCandidateProperty *l) { l_ = l; }

  private:
    bool is_tag_b_;
    DCandidateProperty *d_;
    lCandidateProperty *l_;
    hCandidateProperty *h_;

};

#endif
