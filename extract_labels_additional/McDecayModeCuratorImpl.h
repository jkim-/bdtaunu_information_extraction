
inline void McDecayModeSummary::clear() {
  b_modes_.clear();
  dstar_modes_.clear();
  d_modes_.clear();
}

inline std::unordered_map<McDecayModeSummary::Vertex,
                          std::vector<McDecayModeSummary::Vertex>>&
McDecayModeSummary::b_modes() {
  return const_cast<std::unordered_map<Vertex, std::vector<Vertex>>&>(
           static_cast<const McDecayModeSummary&>(*this).b_modes());
}

inline const std::unordered_map<McDecayModeSummary::Vertex,
                                std::vector<McDecayModeSummary::Vertex>>&
McDecayModeSummary::b_modes() const {
  return b_modes_;
}

inline std::unordered_map<McDecayModeSummary::Vertex,
                          std::vector<McDecayModeSummary::Vertex>>&
McDecayModeSummary::dstar_modes() {
  return const_cast<std::unordered_map<Vertex, std::vector<Vertex>>&>(
           static_cast<const McDecayModeSummary&>(*this).dstar_modes());
}

inline const std::unordered_map<McDecayModeSummary::Vertex,
                          std::vector<McDecayModeSummary::Vertex>>&
McDecayModeSummary::dstar_modes() const {
  return dstar_modes_;
}

inline std::unordered_map<McDecayModeSummary::Vertex,
                          std::vector<McDecayModeSummary::Vertex>>&
McDecayModeSummary::d_modes() {
  return const_cast<std::unordered_map<Vertex, std::vector<Vertex>>&>(
           static_cast<const McDecayModeSummary&>(*this).d_modes());
}

inline const std::unordered_map<McDecayModeSummary::Vertex,
                          std::vector<McDecayModeSummary::Vertex>>&
McDecayModeSummary::d_modes() const {
  return d_modes_;
}

