#include <vector>
#include <iostream>

class NLO_EFT {
 public:

  int n = 41; // number of lines 41
  int p = 23; // number of  columns

  void GetMatrix(float **Matrix);
  int GetMhhCenterValue(float m_hh);
  int GetLine(int m_hh, float **A_values);
  float Rhh(float c2, float c2g, float cg, float kl, float kt, std::vector<float> A);

};
