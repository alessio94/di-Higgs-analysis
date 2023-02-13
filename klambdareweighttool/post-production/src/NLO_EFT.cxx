#include <iostream>
#include <fstream>
#include <cmath>
#include "NLO_EFT.h"
#include <vector>

// Function to get Matrix of values from txt file
void NLO_EFT::GetMatrix(float **Matrix){
  std::string line;
  std::ifstream myfile ("../data/NLO_norm_values.txt");
  std::cout<<" -- Opening input file.."<<std::endl;
  int a = 0; // line index
    
  if (myfile.is_open())
    {
      while ( getline (myfile,line) )
	{
	  std::string temp = "";
	  int b = 0; // column index
	  for(int i=0; i < line.size(); i++){
	    //std::cout<<" i = " << i <<std::endl;

	    if (!isblank(line[i])){
	      std::string d(1, line[i]);
	      temp.append(d);
	      //std::cout<<" appending " << d <<std::endl;
	    } else {
	      Matrix[a][b]= std::stof(temp); //convert string to float
	      //std::cout<<" Found blank space! Full number " << temp << " now b = " << b+1 << std::endl;      
              temp = "";
	      b+=1;
	    }
	  }
	  //std::cout<<" Finished line!  now a = " << a+1 << std::endl;
	  a+=1;
	}
      myfile.close();
    }
  else std::cout << "Unable to open file"; 
}

// Get Mhh central value according to binning
int NLO_EFT::GetMhhCenterValue(float m_hh){
  int mhh_value;
  int i;
  if (m_hh > 1040) {
    mhh_value = 1030;
  } //Giving mhh values > 1030 the weight for mhh = 1030 GeV.
  for (i = 1; i < 41; ++i){
    if ((240+i*20) > m_hh) { 
      mhh_value = (240+i*20)-10;
      //std::cout << "Found m_hh! " << mhh_value << std::endl;
      break;
    }
  }
  return mhh_value;
}


//Find which line the m_hh of the event belongs to and find the corresponding the A values
int NLO_EFT::GetLine(int m_hh, float **A_values){
   
  int m_hh_th[n];
  
  int line = -1;
    //  std::cout << " m_hh value to compare: " <<  m_hh << std::endl;
  for(int i=0; i < n-1; i++)
    {
      //  std::cout << "checking line" << i << std::endl;
      m_hh_th[i] = A_values[i][0];
      //std::cout <<  A_values[i][0] << std::endl;      
      if( m_hh_th[i] == m_hh){
	line = i;
	break;
      }      
      //      std::cout << "Line = " << line+1 << std::endl;;
    }
    
  return line;
}


//Calculate the value of the Polynomial
float NLO_EFT::Rhh(float c2, float c2g, float cg, float kl, float kt, std::vector<float>A){

  //std::cout << kt << " " << kl << " " << c2 << " " << cg << " " << c2g << std::endl;

  float Poly = A[0]*pow(kt,4) + A[1]*pow(c2,2) + (A[2]*pow(kt,2) + A[3]*pow(cg,2))*pow(kl,2) + A[4]*pow(c2g,2) + ( A[5]*c2 + A[6]*kt*kl )*pow(kt,2)  + (A[7]*kt*kl + A[8]*cg*kl )*c2 + A[9]*c2*c2g  + (A[10]*cg*kl + A[11]*c2g)*pow(kt,2)+ (A[12]*kl*cg + A[13]*c2g )*kt*kl + A[14]*cg*c2g*kl + A[15]*cg*pow(kt,3) + A[16]*c2*kt*cg + A[17]*kt*kl*pow(cg,2)+ A[18]*kt*cg*c2g + A[19]*pow(kt,2)*pow(cg,2) + A[20]*c2*pow(cg,2) + A[21]*kl*pow(cg,3) + A[22]*c2g*pow(cg,2);

  return Poly;

}


