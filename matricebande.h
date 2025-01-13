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
#endif

#include "matrice.h"

using namespace std;

struct mgrad
{
    vector<vector<double>> x;
    int k;
    vector<double> nrb; 
};

/// Operateur << permetant d'afficher x[k] avec k le nombre d'iterations avant la condition d'arret
ostream& operator << (ostream&, const mgrad &mg )
{
    vector<double> a= mg.x[mg.k];
    cout << "x[" << mg.k << "]=" << a;
}

class matricebande : public matrice
{
private:
    vector<int> indice;
    vector<double> b;
    vector<vector<double>> ttab;
    vector<int> tind;
public:
    matricebande() {};                         // constructeur par defaut
    matricebande(int,int);                     // constructeur en donnant 2 dimensions
    matricebande(const matricebande&);         // constructeur par recopie
    ~matricebande() {};                        // destructeur par defaut
    
    vector<int> ind() const {return this->indice;}    // retourne le champ privé indice
    vector<int> ind(vector<int>);                     // remplit le champ privé indice

    matricebande laplacien(int);                      // assemblage du laplacien TP3, question (1.1)
    matricebande laplacien_rho(int);
    
    void assemblageT(int,double);
    
    friend vector<double> smbr(matricebande &,int);
    friend vector<double> smbr_rho(matricebande &,int);
    
    vector<double> preconditionne(vector<double>);
    
    // rho(x,y)=10 si (x,y) dans S ; rho(x,y)=0 sinon; rho(-1/2,y)=rho(1/2,y) ; rho(N+1/2,y)=rho(N+3/2,y)
    double rho(double i,double j, int N)
    {
        // prolongement symétrique
            if(i==-0.5)
            {
                return rho(0.5,j,N);
            }
       
            if(i==(double)(N)+1.5)
            {
                return rho((double)(N)+0.5,j,N);
            }
        
        // Calcul de rho(x,y)
        double h = 1/((double)(N)+1);
        double x_i=i*h;
        double y_j=j*h;
        
        // On regarde si (x_i,y_j) est dans S
        if(0.25 <= x_i && x_i <= 0.75 && 0.4 <= y_j && y_j <=0.6)
        {
            //cout << i << " "<< j << endl;
            return 10;
        }
        
        else
        {
            return 1;
        }
    }
    
    mgrad steepest_descent(vector<vector<double>>, vector<double>, double);
    mgrad gradient_pas_fixe(vector<vector<double>>, vector<double>, double,double);
    mgrad PCG(vector<vector<double>>, double);
    vector<double> operator*(vector<double>);
};

vector<double> smbr(matricebande &mb,int N)
{
    int n=mb.dim1();
    double h=1/(double(N)+1);
    double hh=h*h;
    vector<double> v(n,0);
    
    // On remplit les N+2 premiers termes
    for(int i=0; i<(N+2);i++)
    {
        v[i]=100/hh;
    }
    
    // Division du 1er terme et du N+2 eme termes
    v[0]=v[0]/2;
    v[N+1]=v[N+1]/2;
    
    // On remplit les N+2 derniers termes
    for(int i=n-N-2; i<n;i++)
    {
        v[i]=50/hh;
    }
    
    // Division du 1er termes des N+2 derniers termes et du dernier terme
    v[n-N-2]=v[n-N-2]/2;
    v[n-1]=v[n-1]/2;
    
    // On remplit b de la classe matricebande
    mb.b=v;
    
    return v;
}

vector<double> smbr_rho(matricebande &mb,int N)
{
    int n=mb.dim1();
    double h=1/(double(N)+1);
    double hh=h*h;
    vector<double> v(n,0);
    
    // On remplit les N+2 premiers termes
    for(int i=0; i<(N+2);i++)
    {
        v[i]=(mb.rho(double(i),0.5,N)*100)/(hh);
    }
    
    // Division du 1er terme et du N+2 eme termes
    v[0]=v[0]/2;
    v[N+1]=v[N+1]/2;
    
    // On remplit les N+2 derniers termes
    for(int i=n-N-2; i<n;i++)
    {
        v[i]=(50*mb.rho(double(i),double(N)+0.5,N))/(hh);
    }
    
    // Division du 1er termes des N+2 derniers termes et du dernier terme
    v[n-N-2]=v[n-N-2]/2;
    v[n-1]=v[n-1]/2;
    
    // On remplit b de la classe matricebande
    mb.b=v;
    
    return v;
}
//------------------------------------------------------
//------------------------------------------------------
// Constructeur
matricebande::matricebande(int n, int d) : matrice(n,d)
{this->indice.resize(d);} // remplit le champ privé indice avec 0

//------------------------------------------------------
// Constructeur par recopie
matricebande::matricebande(const matricebande& A) : matrice(A)
{this->indice=A.ind();}

//-------------------------------------------------------
// Remplit le champ privé indice avec le vecteur v
vector<int> matricebande::ind(vector<int> v)
{return this->indice=v;} //

// On créer l'operateur *
vector<double> matricebande::operator*(vector<double> x)
{
    int n=this->dim1();
    assert(x.size() == n);
    int nb_bande=indice.size();
    vector<double> res(n,0.);
    
    for(int i=0; i<n;i++)
    {
        for(int j=0; j<nb_bande;j++)
        {
                if(indice[j]==0)
                {
                    res[i]=res[i]+(*this)(i,j)*x[i];
                }
                else
                {
                    if(abs(indice[j])+i < n)
                        res[i]=res[i]+(*this)(abs(indice[j])+i,j)*x[abs(indice[j])+i];
                        
                    if((n-1)-abs(indice[j])-i>=0)
                        res[(n-1)-i]=res[(n-1)-i]+(*this)((n-1)-i,j)*x[(n-1)-abs(indice[j])-i];
                }
        }
        
    }
    return res;
}

//-------------------------------------------------------
// Construit le stockage bande de la matrice du laplacien 2D
//-------------------------------------------------------
matricebande matricebande::laplacien(int n){
    
    int N=(n+2)*n;
    double h = 1./(n+1), hh=pow(h,2);

    vector<int> v;
    v.push_back(-n-2);v.push_back(-1); v.push_back(0);
    
    matricebande A(N,3);
    A.ind(v);
        
    int ligne;
 
    // Ici j==0
    for (int i=0; i<n+2; i++){ // CL de Neumann sur les bords verticaux
        ligne=i;
        if (i==0)
            A(ligne,2)= 4./hh;
        else if (i<n+1){
            A(ligne,1)=-1./hh;
            A(ligne,2)= 4./hh;
        }
        else{
            A(ligne,1)=-2./hh;
            A(ligne,2)= 4./hh;
        }
    }
    // CL de Dirichlet sur les bords horizontaux, j==0 déjà traité
    for (int j=1; j<n; j++){
        for (int i=0; i<n+2; i++){ // CL de Neumann sur les bords verticaux
            ligne=(n+2)*j+i;
            if (i==0){
                A(ligne,0)=-1./hh;
                A(ligne,2)= 4./hh;
            }
            else if (i<n+1){
                A(ligne,0)=-1./hh;
                A(ligne,1)=-1./hh;
                A(ligne,2)= 4./hh;
            }
            else{
                A(ligne,0)=-1./hh;
                A(ligne,1)=-2./hh;
                A(ligne,2)= 4./hh;
            }
        }
    }
    cout << "matrice du laplacien avant symetrisation \n";
    cout << A << endl;

    // Symetrisation de A
    for (int j=0; j<n; j++){
        ligne=(n+2)*j;
        A[ligne]=A[ligne]*0.5; // modification de toute la ligne
        ligne=(n+2)*j+n+1;
        A[ligne]=A[ligne]*0.5;
    }
    
    return A;
}

matricebande matricebande::laplacien_rho(int n){
    
    int N=(n+2)*n;
    double h = 1./(n+1), hh=pow(h,2);

    vector<int> v;
    v.push_back(-n-2);v.push_back(-1); v.push_back(0);
    
    matricebande A(N,3);
    A.ind(v);
        
    int ligne;
    int j=1;
    int i=0;
    
    vector<double> w(15,0.);
 
    for(int k=0;k<N;k++)
    {
        // i et j sont les entiers de T(i,j)
        // Sous-diagonale la plus basse
        if(k>=n+2)
        {
            A(k,0)=-rho((double)(i),(double)(j)-0.5,n)/hh;
        }
        else
        {
            A(k,0)=0;
        }
        
        // Diagonale
        A(k,2)=rho((double)(i)+0.5,(double)(j),n)+rho((double)(i)-0.5,(double)(j),n)+rho((double)(i),(double)(j)+0.5,n)+rho((double)(i),(double)(j)-0.5,n);
        A(k,2)=A(k,2)/hh;
        // Sous diagonale
        if(k!=0 & (k+1)%(n+2)!=1)
        {
            // Si k+1 est un multiple de n+2
            if ((k+1)%(n+2)==0)
            {
                A(k,1)=-rho((double)(i)-0.5,(double)(j),n)-rho((double)(i)+0.5,(double)(j),n);
            }
            else
            {
                A(k,1)=-rho((double)(i)-0.5,j,n);
            }
            
            A(k,1)=A(k,1)/hh;
        }
        
        // Indice j et i
        i++;
            // Si k+1 est un multiple de n+2
            if((k+1)%(n+2)==0)
            {
                j++;i=0;
            }
    }
    
    cout << "matrice du laplacien_rho avant symetrisation \n";
    cout << A << endl;
    
    // Symetrisation de A
    for (int l=0; l<n; l++){
        ligne=(n+2)*l;
        A[ligne]=A[ligne]*0.5; // modification de toute la ligne
        ligne=(n+2)*l+n+1;
        A[ligne]=A[ligne]*0.5;
    }
    
    return A;
}

mgrad matricebande::steepest_descent(vector<vector<double>> x, vector<double> b, double tol)
{
    // taille du vecteur x0
    int m = x[0].size();
    // nombre max d'iter
    int itermax = 10*m;
    //r=r0
    vector<double> r = b-(*this)*x[0];
    double normr=norme_inf(r);
    //rtol=tol*||r0||
    double rtol=normr*tol;
    // srtol = ||r0||/tol condition erreur OVERFLOW
    double srtol=normr/tol;
    double normb = norme_inf(b);
    // nrb vecteur des ||rk||/||b||
    vector<double> nrb;
    // nrb[0]=||r0||/||b||
    nrb.push_back(normr/normb);
    double alpha=0.;
    int k=0;
    
    while (normr > rtol && k<itermax)
    {
        alpha=(r*r)/(r*((*this)*r));
        k++;
        x.push_back(x[k-1]+(r*alpha));
        r=r-((*this)*(r*alpha));
        normr=norme_inf(r);
        nrb.push_back(normr/normb);
        
        if(normr >= srtol)
            cout << k << endl;
        assert(normr < srtol && "OVERFLOW");
    }
    
    // strucutre mgrad
    mgrad mg;
    mg.x=x;
    mg.nrb=nrb;
    mg.k=k;
    
    return mg;
}

mgrad matricebande::gradient_pas_fixe(vector<vector<double>> x, vector<double> b, double tol,double alpha)
{
    int m =  x[0].size();
    // nombre max d'iter
    int itermax = 10*m;
    // r=r0
    vector<double> r = b-(*this)*x[0];
    double normr=norme_inf(r);
    // rtol=||r0||*tol
    double rtol=normr*tol;
    // srtol=||r0||/tol
    double srtol=normr/tol;
    double normb = norme_inf(b);
    // vecteur de ||rk||/||b||
    vector<double> nrb;
    // nrb[0]=||r0||/||b||
    nrb.push_back(normr/normb);
    int k=0;
    
    while (normr > rtol && k<itermax)
    {
        k++;
        x.push_back(x[k-1]+(r*alpha));
        r=r-(*this)*r*alpha;
        normr=norme_inf(r);
        nrb.push_back(normr/normb);
        if(normr >= srtol)
            cout << k << endl;
        assert(normr < srtol && "OVERFLOW");
    }
    
    // structure mgrad
    mgrad mg;
    mg.x=x;
    mg.nrb=nrb;
    mg.k=k;
    
    return mg;
}

void matricebande::assemblageT(int choix,double omega)
{
    // On remet à 0 tind et ttab au cas ou il sont deja remplis
    int n = this->dim1();
    this->tind.clear();
    this->ttab.clear();
    
    
    switch(choix)
    {
        // choix 1 pour C
        case 1:
                tind.push_back(0);
                ttab.push_back(vector<double>(n,1));
            break;
// -------------------------------            
        // choix 2 pour C
        case 2:
        {
                tind.push_back(0);
                ttab.push_back(vector<double>(n,0));

                // pas d'utilité (k=2)
                    int ind_size = this->indice.size();
                    
                    assert(indice.empty() == false);
                    
                    // position de la diagonale k 
                    int k=0;
                    while(indice[k] != 0 && k <indice.size())
                    {k++;}
                
                // Calcul de T
                for(int i=0;i<n;i++)
                {
                    this->ttab[0][i]=sqrt(abs( (*this)(i,k)  ));
                }
            
            break;
        }
// -------------------------------------
        // choix 3 pour C
        case 3:
            {
                tind=indice;
                ttab = vector<vector<double>>(3,vector<double>(n,0.));
                
                // On calcul le terme de la diagonale i de T puis ensuite tout ses termes dans la même colonne
                for(int i=0;i<n;i++)
                {
                     /// Calcul de Tii
                    double sum=0;
                          
                    //Calcul de somme
                    sum=pow(ttab[0][i],2)+pow(ttab[1][i],2);
                            
                    ttab[2][i]=sqrt((*this)(i,2)*(1+omega)-sum);
                    
                    /// Calcul des coefficents en dessous de la diagonale pour la colonne i
                    if (i+1<n)
                        ttab[1][i+1]=(*this)(i+1,1)/ttab[2][i];;
                    
                    int D = abs(indice[0]);
                    if (i+D<n)
                    {
                        ttab[0][i+D]=(*this)(i+D,0)/ttab[2][i];
                    }       
                }
            }
            break;
        
        // Par défaut le choix 1
        defaut:
            tind.push_back(0);
            ttab.push_back(vector<double>(n,1));
        break;
    }
}

vector<double> matricebande::preconditionne(vector<double> r)
{
    assert(tind.empty() == false);
    int n = ttab[0].size();
    
    assert(r.size()==n);
    

    
    vector<double> y(n,0);
    int ind_size=tind.size();
    
    int did=0;
    while(tind[did] != 0 && did < tind.size())
    {did++;}
    
    // Descente Ty=r
    for(int i=0;i<n;i++)
    {
        double sum=0;
        for(int k=0; k<ind_size;k++)
        {
            int id = i-abs(tind[k]);
            if(id>=0 && abs(tind[k])<=i)
            {
                sum=sum+ttab[k][i]*y[id];
            }
        }
        y[i]=(r[i]-sum)/ttab[did][i];
    }
    
    vector<double> z(n,0);
        
    // Remontée Tt*z=y (Tt transpose de T)
    for(int i=n-1;i>=0;i--)
    {
        double sum=0;
        for(int k=0; k<ind_size;k++)
        {
            int id = i+abs(tind[k]);
            if(id<n && k!=did)
            {
                sum=sum+ttab[k][id]*z[id];
                //cout << "ttab["<<k<<"][" << id << "]*z[" << id << "]" << endl;
            }
        }
        z[i]=(y[i]-sum)/ttab[did][i];
    }

    
    return z;
}

mgrad matricebande::PCG(vector<vector<double>> x, double tol)
{
    mgrad mg;
    vector<double> v;
    vector<double> r;
    vector<double> z;
    vector<double> p;
    vector<double> q;
    double gamma;
    double alpha;
    double beta;
    
    // Initialisation de la méthode
    r=b-(*this)*x[0];
    z=this->preconditionne(r);
    p=z;
    gamma=z*r;
    double normb=norme_inf(b);
    v.push_back(norme_inf(r)/normb);
    
    // pour la condition d'arret
    double tol2gam = pow(tol,2)*gamma;
    
    int k=0;
    while (gamma >=tol2gam )
    {
        q=(*this)*p;
        alpha=gamma/(p*q);
        x.push_back(x[k]+p*alpha);
        r=r-q*alpha;
        v.push_back(norme_inf(r)/normb);
        z=this->preconditionne(r);
        beta=1/gamma;
        gamma=z*r;
        beta=beta*gamma;
        p=z+p*beta;
        k++;
    }
    
    //structure mgrad
    mg.x=x;
    mg.nrb=v;
    mg.k=k;
    
    return mg;
}