#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <math.h>  
#include <vector>
#include <list>
#include <iterator>   
#include <stdlib.h>     
#include <time.h>       
#include <chrono>   
#include "funcoes.h"
#include <ilcplex/ilocplex.h>

using namespace std;

int main (int argc, char *argv[]) {
ifstream arq(argv[1]);
		if (!arq) {cerr << "Error  \n"; exit(0);}
 
srand (time(NULL));
        clock_t inicio_CPU, fim_CPU;   
        inicio_CPU = clock();

     
int m;
        int n;
        int i;
        int AUX;
        int id;
        arq >> AUX;
        if (AUX == 100) //beasley instances
        {
            id = 1;
            m = AUX;
            arq >> n;
        }
              if (AUX > 100) //TBED 1 instances
        {
            id = 2;
            n = AUX;
            arq >> m;
        }
        if (AUX < 100) //yang instances //gaadegard
        {
            id = 3;
            m = AUX;
            arq >> n;
        }

        vector<double> p(m); // capacity
        vector<double> f(m); // fix cost
        vector<double> d(n); // demand
        vector<vector<double>> c(n, vector<double>(m)); // unit costs
        vector<double> pen(m); // penality (less and equal 0 in all algorithm)
        vector<vector<int>> matriz_clientes_ordenados(m, vector<int>(n)); 
                //for each costumer presents the vector of locations ordered according to the cost of allocation
        vector<vector<int>> matriz_facilidades_ordenadas(n, vector<int>(m));
                //for each location presents the vector of costumers ordered according to the cost of allocation
bool coeficientes_inteiros = false;

//read values ​​from instances

       if (id == 1)  //or 4
       {
        vector<vector<double>> caux(n, vector<double>(m)); 
        for (int j = 0; j <  m; j++){
        arq >> p[j]; 
        arq >> f[j]; 
        }

for (int i = 0; i <  n; i++){
arq >> d[i];     
        for (int j = 0; j < m; j ++)
        {
        arq >> caux[i][j] ;
        }
        }


       for (int i = 0; i <  n; i++){
                for (int j = 0; j <  m; j++){
                matriz_clientes_ordenados[j][i] = i;
                matriz_facilidades_ordenadas[i][j] = j;
                c[i][j] = (double) caux[i][j]/d[i];

                }

             }








       }
       if (id == 2) //tbed1
    {
      vector<vector<double>> caux(m, vector<double>(n));
    for (int i = 0; i <  n; i++){
arq >> d[i];
        }
    for (int j = 0; j <  m; j++){
arq >> p[j];
        }
    for (int j = 0; j <  m; j++){
arq >> f[j];
        }
    for (int j = 0; j < m; j ++){
   for (int i = 0; i <  n; i++)
        {
arq >> caux[j][i];
        }
    }


                for (int j = 0; j < m; j ++){
   for (int i = 0; i <  n; i++)
        {

             c[i][j] = caux[j][i];
                   matriz_clientes_ordenados[j][i] = i;
                matriz_facilidades_ordenadas[i][j] = j;
        }    
    }




    }
       if (id == 3)
        {
        coeficientes_inteiros = true;      

            for (int j = 0; j <  m; j++)
            {
                arq >> p[j];
                arq >> f[j];
            }
            for (int i = 0; i <  n; i++){
                arq >> d[i];
            }
       
        vector<vector<double>> caux(m, vector<double>(n)); 
        for (int j = 0; j < m; j ++){
            for (int i = 0; i <  n; i++)
            {
arq >> caux[j][i];
        }
    }
        for (int j = 0; j < m; j ++){
   for (int i = 0; i <  n; i++)
        {

       
             c[i][j] = (double) caux[j][i]/d[i];
                    matriz_clientes_ordenados[j][i] = i;
                matriz_facilidades_ordenadas[i][j] = j;
        }
    }
    }

cout << "End of data reading" << endl;

//  parameters
double alpha = 1.5;
//irace = 0.5  1    1.5   
double beta = 0.2; 
//irace = 0.25  0.5  0.75  
double gamma = 0.008; 
//irace = 0   0.004    0.008 
double delta = 0.25;
//irace 0.05 0.10 0.15
int ils_max1 = 150;  
 //irace 100 150 200;
int ils_max2 = 90; 
 //irace 90, 120, 150; 
int aux = round(gamma*n);
int constante_trocas = max(aux, 1);  

double tilim2 = 200 ; 
double tilim1 = 1000;  
double soma_das_demandas = 0;  //sum of demands
double menor_demanda = d[0];   // lower demand
for (int i = 1; i < n; i++)
{
   soma_das_demandas = soma_das_demandas + d[i];
   if (d[i] < menor_demanda)
   {
    menor_demanda = d[i];
   }

}

clientes_ordenados( m, n,  c,matriz_clientes_ordenados);
facilidades_ordenadas( m, n,  c,matriz_facilidades_ordenadas);

int contagem = 0;
vector<int> vetor_abertas;
int tamanho_instancia = n;

vector<list<int>> s(m); // solution type 1: list for each location the customers
double fo; // best objective value
for (int i = 0; i < m; i++)
{
pen[i] = -p[i];   
}
list<int> abertas;  //open facilities
list<int> fechadas;   // closed facilities
vector<int> atende(n);   //solution type 2: assigns to each node costumer the facility that serves it
// run the linear relaxation 
IloEnv env2;
IloModel mod2(env2);
IloCplex cplex2(mod2);
IloNumVarArray y2(env2, m, 0, 1, ILOFLOAT); 
IloArray<IloNumVarArray> x2(env2, n);
for(int i = 0; i < n; i++) {  
			 x2[i] = IloNumVarArray(env2, m, 0, 1, ILOFLOAT);
}
// objective *****************************************************
IloExpr expfo2(env2);
for (int j = 0; j <  m; j++){
	expfo2 += f[j] * y2[j];
	for(int i = 0; i < n; i++){
		expfo2 += c[i][j] * x2[i][j] * d[i];
	}
}  
IloAdd(mod2, IloMinimize(env2, expfo2));
expfo2.end();
// assigment constraints  *****************************************************************
for (int i = 0; i < n; i++){
			IloExpr r22(env2);
			for (int j = 0; j < m; j++){
				r22 += x2[i][j];
			}
			mod2.add(r22 == 1);
			r22.end();
		} 
// capacity constraints *****************************************************************
			   for (int j = 0; j < m; j++){
			IloExpr r32(env2);
			for (int i = 0; i < n; i++){
				r32 += d[i]*x2[i][j];
			}
			mod2.add(r32 <= p[j]*y2[j]);
			r32.end();
		} 
// redundant constraints *****************************************************************
			   for (int j = 0; j < m; j++){
			
			for (int i = 0; i < n; i++){
               mod2.add(y2[j] >= x2[i][j]);
			}
			
			
		}
cplex2.setWarning(env2.getNullStream()); // Eliminar warnings
cplex2.setOut(env2.getNullStream()); /// Eliminar os logs do solver
cout << "Running LP(SSCFLP)..." << endl;
cplex2.solve();
double otimo_problema_relaxado = cplex2.getObjValue();
vector<int> melhor_atende_relaxacao(n); // selects for each customer the location that received the most demand

for (int i = 0; i < n; i++)
{
    melhor_atende_relaxacao[i] = 0;
    for (int j = 0; j < m; j++)
    {
       if (cplex2.getValue(x2[i][j]) >  cplex2.getValue(x2[i][melhor_atende_relaxacao[i]])    )
       {
        melhor_atende_relaxacao[i] = j;
       }
    }
    
}
//////////////////////////////////  initial procedure for identifying promising locations  line1  of pseudocode

float media_y = 0; // average of reduced costs
float soma_y = 0;  // sum of reduced costs
for (int j = 0; j < m; j++)
{
    soma_y += cplex2.getReducedCost(y2[j]);
}
media_y = (float) soma_y/m;

float media_y2 = 0;
if (media_y > 0)
{
media_y2 = media_y;
}

vector<int> vgrupo1;
list<int> grupo1;  //y[j] > 0.99   
list<int> grupo2;  
vector<int> vgrupo2;    
list<int> grupo2b;  
vector<int> vgrupo2b;   
list<int> grupo3;  //  y[j] == 0 
vector<int> vgrupo3;   


for (int j = 0; j < m; j++)
{
if (cplex2.getValue(y2[j]) > 0.9999)
{
    grupo1.push_back(j);
    vgrupo1.push_back(j);

    
}
else
{
    if (  ( cplex2.getValue(y2[j]) > 0) && (cplex2.getReducedCost(y2[j]) == 0 ) )
    {
        grupo2.push_back(j);
        vgrupo2.push_back(j); 
         
    }
    if (  ( cplex2.getValue(y2[j]) == 0) && (cplex2.getReducedCost(y2[j]) == 0 ) )
    {
        grupo2b.push_back(j);
        vgrupo2b.push_back(j); 
         
    }
       if (  ( cplex2.getValue(y2[j]) == 0)  && (cplex2.getReducedCost(y2[j]) > 0 )   )
    {
        grupo3.push_back(j);
        vgrupo3.push_back(j); 
         
    }

}
 
}
float mediah = 0;
float somah = 0;
for (int j = 0; j < vgrupo3.size(); j++)
{
     somah += cplex2.getReducedCost(y2[vgrupo3[j]]);
}
mediah = (float) somah/vgrupo3.size();

     for (int i = 0; i < vgrupo2.size(); i++)
     {
     int menor_custo = i;
     for (int j = i; j < vgrupo2.size(); j++)
     {
     if ( cplex2.getValue(y2[vgrupo2[j]])  >  cplex2.getValue(y2[vgrupo2[menor_custo]]) )
        {
        menor_custo = j;    
        }    
    }

    if (menor_custo != i)
    {
    int temp = vgrupo2[i];     
    vgrupo2[i] = vgrupo2[menor_custo];
    vgrupo2[menor_custo] = temp;
    }
     }

     for (int i = 0; i < vgrupo2b.size(); i++)
     {
     int menor_custo = i;
     for (int j = i; j < vgrupo2b.size(); j++)
     {
     if ( (double)f[vgrupo2b[j]]/p[vgrupo2b[j]]   <  (double)f[vgrupo2b[menor_custo]]/p[vgrupo2b[menor_custo]] )
        {
        menor_custo = j;  
        }    
    }

    if (menor_custo != i)
    {
    int temp = vgrupo2b[i];     
    vgrupo2b[i] = vgrupo2b[menor_custo];
    vgrupo2b[menor_custo] = temp;
    }
     }


     for (int i = 0; i < vgrupo3.size(); i++)
     {
     int menor_custo = i;
     for (int j = i; j < vgrupo3.size(); j++)
     {
     if ( cplex2.getReducedCost(y2[vgrupo3[j]]) <  cplex2.getReducedCost(y2[vgrupo3[menor_custo]]) )
        {
        menor_custo = j;  
        }    
    }

    if (menor_custo != i)
    {
    int temp = vgrupo3[i];     
    vgrupo3[i] = vgrupo3[menor_custo];
    vgrupo3[menor_custo] = temp;
    }
     }

vector<int> facilidades_potenciais;  /////// promising locations


for (int i = 0; i < vgrupo1.size(); i++)
{
    facilidades_potenciais.push_back(vgrupo1[i]);
}
for (int i = 0; i < vgrupo2.size(); i++)
{
    facilidades_potenciais.push_back(vgrupo2[i]);
}

for (int i = 0; i < vgrupo2b.size(); i++)
{
    facilidades_potenciais.push_back(vgrupo2b[i]);
}
for (int i = 0; i < vgrupo3.size(); i++)
{
    if (cplex2.getReducedCost(y2[vgrupo3[i]]) < delta*mediah)
    {
        facilidades_potenciais.push_back(vgrupo3[i]);
    }
}

double soma_capacidades = 0;

int tamanho_abertas_grupo1 = facilidades_potenciais.size();

//************************************ //  construct initial solution    line 2 of pseudocode
cout << "Generating Initial Solution..." << endl;     
vector<int> vetor_clientes(n);

int contador = 0;
for (int i = 0; i < n; i++)
{
    vetor_clientes[i] = i;
}
//cout << "Adiciona 1: "; 
ordena_clientes_decrescente_demanda( vetor_clientes,d);
while (soma_capacidades < soma_das_demandas)
{
    //cout << facilidades_potenciais[contador] << " "; 
    abertas.push_back(facilidades_potenciais[contador]);
    //fechadas.remove(facilidades_potenciais[contador]);
    soma_capacidades += p[facilidades_potenciais[contador]];
    if (soma_capacidades < soma_das_demandas)
    {
     contador = contador + 1;
    }
    
}
//cout << endl;
bool conseguiu_alocar = false;
while(conseguiu_alocar == false)
{
  //cout << "Adiciona 2 "; 
  aloca_clientes(fo, m, n,c,  matriz_facilidades_ordenadas, vetor_clientes,p, d,conseguiu_alocar,  abertas, pen, atende, f, menor_demanda, soma_das_demandas, melhor_atende_relaxacao);

  if (conseguiu_alocar == false)
  {
    
    contador = contador + 1;
    abertas.push_back(facilidades_potenciais[contador]);
    //cout << facilidades_potenciais[contador] << " ";
    //fechadas.remove(facilidades_potenciais[contador]);
  }  

}
cout << "fo after initial solution: " << fo << endl;


vector<int> todas_facilidades;
for (int i = 0; i < m; i++)
{
    todas_facilidades.push_back(i);
}

list<int> fechadas_disponiveis;
for (int j = 0; j < todas_facilidades.size(); j++)
{
   if (!esta_na_lista(abertas, todas_facilidades[j]) )
   {
    fechadas_disponiveis.push_back(todas_facilidades[j]);
   } 
}

list<int> abertas_disponiveis;
cria_copia_lista(abertas_disponiveis,abertas);
cria_copia_lista(fechadas,fechadas_disponiveis);
vector<int> novas_facilidades;

//////////////////////////////////////   applies procedure for opening facilities   line 3 of pseudocode
cout << " apply facility opening procedure..." << endl;   

int quantidade_abertas_movimento1 = -1;
movimentos_abre_apos_solucao_inicial_best_improvement( fo,m,  n,c,  matriz_facilidades_ordenadas, vetor_clientes, p,d,conseguiu_alocar, abertas_disponiveis ,pen,atende, f, fechadas_disponiveis, menor_demanda, soma_das_demandas, novas_facilidades, facilidades_potenciais, quantidade_abertas_movimento1, melhor_atende_relaxacao);
cria_copia_lista(abertas,abertas_disponiveis);
cria_copia_lista(fechadas,fechadas_disponiveis);

cout << "fo after opening procedure: " << fo << endl;

for (int j = 0; j < m; j++)
{
    s[j].clear();
}

for (int i = 0; i < n; i++)
{
   s[atende[i]].push_back(i);
}


for (int i = 0; i < n; i++)
{

    s[atende[i]].push_back(i);
}

fechadas.clear();
abertas.clear();

for (int j = 0; j < m; j++)
{
    if ( s[j].empty() )
    {
        fechadas.push_back(j);
    }
    else
    {
        abertas.push_back(j);
    }
}

int soma_clientes = 0;
double fstar2 = 0;
int tam_grupo3 = grupo3.size();
int tamanho_facilidades_potenciais = facilidades_potenciais.size();
int tam_facilidades_potenciais = facilidades_potenciais.size();
abertas_disponiveis.clear();
for (std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++ )
  {
  abertas_disponiveis.push_back(*k);
 }
for (int i = 0; i < n; i++)
{
   vetor_clientes[i] = i;
}
bool factivel = true;

///////////////////////////////////////////////// applies VND procedure line 4 of pseudocode

cout << " apply VND procedure..." << endl;   
VND_factivel_1_2_3(m, n, s ,fo, pen, atende, c,d,f, abertas,fechadas, vetor_clientes ,  factivel, abertas_disponiveis, p, menor_demanda) ;

cout << "fo after vnd:" << fo << endl;
/////////////////////////////////////////////// start phase I   

cout << " start phase I " << endl;   
vector<vector<int>> start(n, vector<int>(m));
/****************************************************************************************************/
int inter = 0;    
bool parada2 = false;     
double gap = 100;             
double gap2 = 100;          
double lb = 0;             
double lb2 = 0;           
double fo_antes2 = fo;         
int conta = 0;
bool aux2 = false;
bool otimo_reduced_sscflp = false;
//cout << "PHASE I" << endl;
clock_t fim_CPU2 = clock();
aux2 = false;
fim_CPU2 = clock();

vector<vector<int>> start1(n, vector<int>(facilidades_potenciais.size()));
while (parada2 == false) 
{
double fo_antes2 = fo;

otimo_reduced_sscflp = false;
IloEnv env3;
IloModel mod3(env3);
IloCplex cplex3(mod3);
IloNumVarArray y3(env3, facilidades_potenciais.size(), 0, 1, ILOBOOL); 
IloArray<IloNumVarArray> x3(env3, n);
for(int i = 0; i < n; i++) {  
    x3[i] = IloNumVarArray(env3, facilidades_potenciais.size(), 0, 1, ILOBOOL);
}

// declaração da expressão da funcao objetivo*****************************************************
IloExpr expfo(env3);
for (int j = 0; j <  facilidades_potenciais.size(); j++){
	expfo += f[facilidades_potenciais[j]] * y3[j];
	for(int i = 0; i < n; i++){
		expfo += c[i][facilidades_potenciais[j]] * x3[i][j] *d[i];
	}
}  
IloAdd(mod3, IloMinimize(env3, expfo));
expfo.end();

//restricao de indivisibilidade*****************************************************************
for (int i = 0; i < n; i++){
			IloExpr r3(env3);
			for (int j = 0; j < facilidades_potenciais.size(); j++){
				r3 += x3[i][j];
			}
			mod3.add(r3 == 1);
			r3.end();
		} 
//restricao de de capacidade *****************************************************************
			   for (int j = 0; j < facilidades_potenciais.size(); j++){
			IloExpr r3(env3);
			for (int i = 0; i < n; i++){
				r3 += d[i]*x3[i][j];
			}
			mod3.add(r3 <= p[facilidades_potenciais[j]]*y3[j]);
			r3.end();
		} 


if (  id == 1  )  // redundant sentences for OR-4 data sets.
{
  for (int j = 0; j < facilidades_potenciais.size(); j++){

for (int i = 0; i < n; i++){
               mod3.add(y3[j] >= x3[i][j]);
}


}
}

IloExpr r5(env3);
for (int j = 0; j <  facilidades_potenciais.size(); j++){
	r5 += f[facilidades_potenciais[j]] * y3[j];
	for(int i = 0; i < n; i++){
		r5 += c[i][facilidades_potenciais[j]] * x3[i][j] *d[i];
	}
} 
mod3.add(r5 >= lb2);
r5.end();

for (int i = 0; i < n ; i++)
{
    for (int j = 0; j < facilidades_potenciais.size(); j++)
    {
    if (atende[i] == facilidades_potenciais[j])
    { 
      start1[i][j] = 1;

    }
    else
    {
       start1[i][j] = 0;
    }
    }
    
  }

IloNumVarArray startVar(env3);
IloNumArray startVal(env3);
for (int i = 0; i < n; i++)
    {
         for (int j = 0; j < facilidades_potenciais.size(); j++) {
             startVar.add(x3[i][j]);
             startVal.add(start1[i][j]);
         }
    }
     cplex3.addMIPStart(startVar, startVal);
    
     startVal.end();
     startVar.end();

cplex3.setWarning(env3.getNullStream()); // Eliminar warnings
cplex3.setOut(env3.getNullStream()); /// Eliminar os logs do solver
cplex3.setParam(IloCplex::Threads,1);


cplex3.setParam(IloCplex::EpGap, 0.0000001);
cplex3.setParam(IloCplex::TiLim, tilim1);
cout << "Running cplex for Reduced SSCFLP...." << endl;

cplex3.solve();
fstar2 = cplex3.getObjValue();	
lb2 = cplex3.getBestObjValue();	
gap2 = 100*cplex3.getMIPRelativeGap();


if (cplex3.getStatus() == IloAlgorithm::Optimal)
{
    otimo_reduced_sscflp = true;
}


float tempo = (float)(fim_CPU2 - inicio_CPU)/CLOCKS_PER_SEC ;

fo = 0;
for (int j = 0; j < facilidades_potenciais.size(); j++)
{
   s[facilidades_potenciais[j]].clear();
   pen[facilidades_potenciais[j]] = -p[facilidades_potenciais[j]];
   for (int i = 0; i < n; i++)
   {
   if ( abs(cplex3.getValue(x3[i][j])-1) < 0.0001)
   {
       s[facilidades_potenciais[j]].push_back(i);
       atende[i] = facilidades_potenciais[j];
       pen[facilidades_potenciais[j]] = pen[facilidades_potenciais[j]] + d[i];
       fo = fo + c[i][facilidades_potenciais[j]] *d[i];
   }
   }
  // fo = fstar2;
   
}
abertas.clear();
fechadas.clear();
for (int j = 0; j < m; j++)
{
   if (!s[j].empty())
   {
       abertas.push_back(j);
       fo = fo + f[j];
   }
   else
   {
       fechadas.push_back(j);
   }
}


cout << " fo after reduced sscflp: " << fo << endl;

if (otimo_reduced_sscflp == true)
{
    cout << "apply ILS I procedure: " << endl;
    ILS_1_novo2(m, n, s ,fo,pen,atende,  c,  d,f, abertas, fechadas, facilidades_potenciais, p, tamanho_instancia, matriz_clientes_ordenados,menor_demanda, soma_das_demandas, ils_max1);
    cout << "fo after ILS I:" << fo << endl; 
}
else{
        cout << "apply ILS I procedure: " << endl;
        ILS_1_novo(m, n, s ,fo,pen,atende,  c,  d,f, abertas, fechadas, facilidades_potenciais, p, tamanho_instancia, matriz_clientes_ordenados,menor_demanda, soma_das_demandas, ils_max1);
        cout << "fo after ILS I:" << fo << endl; 
}


if (  (gap2 <= 0.1)  &&  ( fo >= 0.99999*fstar2 )  )   
{
    parada2 = true;
}
else{
tamanho_facilidades_potenciais = facilidades_potenciais.size();

cout << "apply ILS II procedure: " << endl;
ILS_2 (m, n, s ,fo,pen, atende, c,d, f,abertas,fechadas, facilidades_potenciais, tam_facilidades_potenciais, p, tamanho_instancia, ils_max2, constante_trocas);
cout << "fo after ILS II:" << fo << endl; 


}
if( (fo >= 0.9999*fo_antes2) )  
{

    parada2 = true;
}

}

cout << "Phase I ended" << endl;
if ( (parada2 == true) && ( otimo_reduced_sscflp == false )   )
{
cout << "Starts phase II" << endl;
parada2 = false;
while (parada2 == false) 
{

double fo_antes2 = fo;

vector<int> vetor_abertas;
double fo1 = 0;
for (std::list<int>::iterator k = abertas.begin() ; k !=  abertas.end() ; k++)
{
vetor_abertas.push_back(*k);
fo1 = fo1 + f[*k]; 
}

IloEnv env;
IloModel mod(env);
IloCplex cplex(mod);
IloNumVarArray y(env, vetor_abertas.size(), 0, 1, ILOBOOL); 
IloArray<IloNumVarArray> x(env, n);
for(int i = 0; i < n; i++) {  
    x[i] = IloNumVarArray(env, vetor_abertas.size(), 0, 1, ILOBOOL);
}

// declaração da expressão da funcao objetivo*****************************************************
IloExpr expfo(env);
for (int j = 0; j <  vetor_abertas.size(); j++){
	//expfo += f[j] * y[j];
	for(int i = 0; i < n; i++){
		expfo += c[i][vetor_abertas[j]] * x[i][j] * d[i];
	}
}  
expfo += fo1;
IloAdd(mod, IloMinimize(env, expfo));
expfo.end();

//restricao de indivisibilidade*****************************************************************
for (int i = 0; i < n; i++){
			IloExpr r2(env);
			for (int j = 0; j < vetor_abertas.size(); j++){
				r2 += x[i][j];
			}
			mod.add(r2 == 1);
			r2.end();
		} 
//restricao de de capacidade *****************************************************************
			   for (int j = 0; j < vetor_abertas.size(); j++){
			IloExpr r3(env);
			for (int i = 0; i < n; i++){
				r3 += d[i]*x[i][j];
			}
			mod.add(r3 <= p[vetor_abertas[j]]*y[j]);
			r3.end();
		} 

IloExpr r5(env);
for (int j = 0; j <  vetor_abertas.size(); j++){
	
	for(int i = 0; i < n; i++){
		r5 += c[i][vetor_abertas[j]] * x[i][j] * d[i];
	}
} 
r5 += fo1;
mod.add(r5 >= lb);  //adiciona restricao do lower bound do subproblema
r5.end();

vector<vector<int>> start1(n, vector<int>(vetor_abertas.size()));

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < vetor_abertas.size(); j++)
    {
       start1[i][j] = 0;
    }
}

for (int i = 0; i < n ; i++)
{
    for (int j = 0; j < vetor_abertas.size(); j++)
    {
    if (atende[i] == vetor_abertas[j])
    { 
      start1[i][j] = 1;

    }
    }
 
}

IloNumVarArray startVar(env);
IloNumArray startVal(env);
for (int i = 0; i < n; i++)
         for (int j = 0; j < vetor_abertas.size(); j++) {
             startVar.add(x[i][j]);
             startVal.add(start1[i][j]);
         }
     cplex.addMIPStart(startVar, startVal);
     startVal.end();
     startVar.end();


cplex.setParam(IloCplex::Threads,1);
cplex.setWarning(env.getNullStream()); // Eliminar warnings
cplex.setOut(env.getNullStream()); /// Eliminar os logs do solver
cplex.setParam(IloCplex::EpGap, 0.000001);
//cplex.setParam(IloCplex::Param::MIP::Strategy::File, 3);
cplex.setParam(IloCplex::TiLim, tilim2);
cout << "Running cplex for GAP...." << endl;
cplex.solve();   
fstar2 = cplex.getObjValue();	
lb = cplex.getBestObjValue();	
gap = 100*cplex.getMIPRelativeGap();

cout << "fo after GAP: " << fo << endl;

	if( fstar2 < fo) // o cplex não alterou solucão
{
fo = fo1;
for (int j = 0; j < vetor_abertas.size(); j++)
{
   s[vetor_abertas[j]].clear();
   pen[vetor_abertas[j]] = -p[vetor_abertas[j]];
   for (int i = 0; i < n; i++)
   {
   if ( abs(cplex.getValue(x[i][j])-1) < 0.0001)
   {
       s[vetor_abertas[j]].push_back(i);
       atende[i] = vetor_abertas[j];
       pen[vetor_abertas[j]] = pen[vetor_abertas[j]] + d[i];
       fo = fo + c[i][vetor_abertas[j]] * d[i];
   }
   }
    
}
}

clock_t parex_CPU = clock();
if (  (gap <= 0.0000099) )   
{
    parada2 = true;
}
else
{
cout << "apply ILS II procedure: " << endl;
tamanho_facilidades_potenciais = facilidades_potenciais.size();
ILS_2(m,n,s,fo, pen,atende,c,d, f,abertas,fechadas,facilidades_potenciais, tam_facilidades_potenciais,p,tamanho_instancia, ils_max2, constante_trocas);
cout << "fo after ILS II " << fo << endl;
}

if( (fo >= fo_antes2) )   

    parada2 = true;
}

} // fim while parada 2 = true;

 // fim if vai resolver o gap

cout << "hils algorithm ended" << endl;

cout << fixed << setprecision(4);
cout << argv[1] << endl;
cout << "objetive value: " << fo ;
fim_CPU = clock();
cout << "  cpu time: " << (float)(fim_CPU - inicio_CPU)/CLOCKS_PER_SEC << endl;
cout << "Solution:" << endl;
for (int i = 0; i < n; i++)
{
    cout << atende[i] << " ";
}
cout << endl;

}//fim algoritmo
