#if !defined (__IOSTREAM_H)
#include <iostream>
#endif
#if !defined (__FSTREAM_H)
#include <fstream>
#endif
#if !defined (__CASSERT_H)
#include <cassert>
#endif
#if !defined(__CMATH_H)
#include <cmath>
#endif
#if !defined (__VECTOR_H)
#include <vector>
#include <cmath>
#endif

#include "vecteur_template.cpp" // A cause du template
#include "matricebande.h"

using namespace std;

int main()
{
    matricebande L;
    L=L.laplacien(3);
    
    cout << "DIM L = " << L.dim1() <<" "<< L.dim2() << "\n";
    
    cout << "Laplacien 2D \n" << L << endl;
    vector<double> x(15,1);
    
    cout << "x: " << x << endl;
    
    cout << "L*x:"<< endl << L*x<< endl;
    cout << "b:"<< smbr(L,3) << endl;
    
    cout << "Methode steepest descent" << endl;
    
    vector<vector<double>> y;
    y.push_back(vector<double>(15,0.));
    mgrad mb1 = L.steepest_descent(y,smbr(L,3),pow(10,-4));
    
    cout << "nombre d'iterations k avant condition d'arret" <<endl;
    ecrire_vect("sd1.txt",mb1.nrb);
    cout << "k=" << mb1.k << endl;
    
    cout << "Methode gradient pas fixe pour j=1,...,5" << endl;
    
    for(int i=1;i<=5;i++)
    {
       mgrad mb = L.gradient_pas_fixe(y,smbr(L,3),pow(10,-4),(double)(i)/(64*5));
       cout << "j=" << i << "  k=" << mb.k << endl;
       ecrire_vect("gpf" + to_string(i) +".txt",mb.nrb);
    }
    
/// 3eme partie
    
    int i=1;
    int N=2;
    cout << "choisissez le systeme lineaire a resoudre par la methode du gradient conjugue avec preconditionnement :" << endl << "(1 pour (1.1), 2 pour (1.2)"<<endl; 
    cin >> i;
    cout << "choisissez le N>1"<<endl;
    cin >> N;
    if(i<=0 & i>2)
        i=1;
    if(N<=1)
        N=2;
    
    matricebande K;
    if(i==1)
    {
    K=K.laplacien(N);
    smbr(K,N);
    }
    else
    {
    K=K.laplacien_rho(N);
    smbr_rho(K,N);
    }
    
    int n = (N+2)*N;
    vector<vector<double>> w;
    w.push_back(vector<double>(n,0.));
    mgrad mb;
    
    K.assemblageT(1,0);
    mb=K.PCG(w,pow(10,-4));
    ecrire_vect("P31"+to_string(i)+".txt",mb.nrb);
    cout << "1:" << mb.k << endl;
    
    w.clear();
    w.push_back(vector<double>(n,0.));
    
    K.assemblageT(2,0);
    mb=K.PCG(w,pow(10,-4));
    ecrire_vect("P32"+to_string(i)+".txt",mb.nrb);
    cout << "2:" << mb.k << endl;
    
    w.clear();
    w.push_back(vector<double>(n,0.));
    
    K.assemblageT(3,0.);
    mb=K.PCG(w,pow(10,-4));
    ecrire_vect("P33"+to_string(i)+".txt",mb.nrb);
    cout << "3:" << mb.k << endl;
    
    w.clear();
    w.push_back(vector<double>(n,0.));
    
    K.assemblageT(3,1/double(N));
    mb=K.PCG(w,pow(10,-4));
    ecrire_vect("P34"+to_string(i)+".txt",mb.nrb);
    cout << "4:" << mb.k << endl;
    
    w.clear();
    w.push_back(vector<double>(n,0.));
    
    K.assemblageT(3,1/sqrt((double)(N)));
    mb=K.PCG(w,pow(10,-4));
    ecrire_vect("P35"+to_string(i)+".txt",mb.nrb);
    cout << "5:" << mb.k << endl;
	return 0;
}
