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

//confere a partir de atende

bool esta_na_lista(const std::list<int>& lista, int numero) {    //verifica se um número está em uma lista
    for (const int& elemento : lista) {
        if (elemento == numero) {
            return true;
        }
    }
    return false;
}

void cria_copia_lista (list<int> &lista_nova, list<int> lista)
{
lista_nova.clear();
for (std::list<int>::iterator k = lista.begin(); k != lista.end(); k++ ){
lista_nova.push_back(*k);
}
}

void cria_copia_vetor_int(vector<int> &vetor_novo, vector<int> vetor)
{
for (int i = 0; i < vetor.size(); i++)
{
   vetor_novo[i] = vetor[i];
}

}

void confere_solucao_s(int m, int n, vector<vector<double>> c,  vector<double> f, vector<list<int>>s, vector<double> d, list<int> abertas, list<int> fechadas, vector<double> pen, vector<double> p)
{
double fo1 = 0; //custos abertura facilidades
double fo2 = 0; // custos atendimento clientes
int soma_clientes = 0;   
int conta = 0; 
vector<double> penalidade(m);
for (int j = 0; j < m; j++)
{
    penalidade[j] = -p[j];
}

for (int j = 0; j < m; j++)
{
     
   s[j].sort();
   s[j].unique();
   if (!s[j].empty())
   {
        fo1 = fo1 + f[j]; 
        for (std::list<int>::iterator k1 = s[j].begin(); k1 != s[j].end(); k1++ ){
            soma_clientes = soma_clientes +1 ;
            fo2 = fo2 + c[*k1][j]*d[*k1];
            penalidade[j] = penalidade[j] + d[*k1];
            }
   }
   if (penalidade[j] > 0)
   {
    conta = conta + 1;
   }
}

double fo3;
  for (std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++ ){
  fo3 = fo3 + f[*k];
  }
int erro = 0;
for (int j = 0; j < m; j++)
{
    if (pen[j] != penalidade[j])
    {
        erro = 1;
        break;
    }
}



cout << fo1 + fo2 << " " << soma_clientes <<  " " << fo1 << " " << fo2 << " " << fechadas.size() + abertas.size() << " " << erro << endl;



}

void confere_solucao_atende (int m, int n, vector<vector<double>> c,  vector<double> f, vector<int> atende, vector<double> d)
{
double fo_confere = 0;
double fo1_confere = 0 ;
double fo2_confere = 0;
vector<int> abertas_auxiliar(m);

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < m; j++)
    {
        if (atende[i] == j)
        {
            fo1_confere = fo1_confere + c[i][j]*d[i];
            abertas_auxiliar[j] = 1;
        }
    }
    
}

for (int j = 0; j < m; j++)
{
    fo2_confere = fo2_confere + abertas_auxiliar[j]*f[j];
}
/*
cout << "abertas atende:" << endl;
for (int j = 0; j < m; j++)
{
    if (abertas_auxiliar[j] == 1)
    {
        cout << j << " ";
    }
}
*/
cout << endl;
cout << "soma dos custos fixos de abertura: " << fo2_confere << endl;
cout << "soma dos custos atendimento: " << fo1_confere << endl;
cout << " fo confere: " << fo1_confere + fo2_confere << " ";
}

bool verifica_factivel( vector<double> pen)
{
    for (int j = 0 ; j < pen.size() ; j++)
    {
        if (pen[j] > 0)
        {
        return false;
        }
    }
    return true;
    
}

void explora_vizinhanca_Movement1_restrito_factivel_first_improvement(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis)
{

//explora toda a vizinhança
//cout << "começa a explorar a vizinhnaça:" << endl;
bool houve_melhora = false;

for (int i = 0; i < vetor_clientes.size(); i++)
{
		int r = rand() % vetor_clientes.size();

		int temp = vetor_clientes[i];
		vetor_clientes[i] = vetor_clientes[r];
		vetor_clientes[r] = temp;
}


for (int i = 0; i < vetor_clientes.size(); i++){
for (std::list<int>::iterator k3 = abertas_disponiveis.begin(); k3 != abertas_disponiveis.end(); k3++ )
{

double penalidade_recebe = pen[*k3] + d[vetor_clientes[i]];
if (penalidade_recebe <= 0)
{



double penalidade_retira = 0;
penalidade_retira = pen[atende[vetor_clientes[i]]] - d[vetor_clientes[i]];

if (       (d[vetor_clientes[i]]*c[vetor_clientes[i]][*k3]  <  d[vetor_clientes[i]]*c[vetor_clientes[i]][atende[vetor_clientes[i]]]  )   && (s[atende[vetor_clientes[i]]].size() > 1.1) && (*k3 != atende[vetor_clientes[i]])  )
{
    houve_melhora = true;
    fo = fo + c[vetor_clientes[i]][*k3]*d[vetor_clientes[i]] - c[vetor_clientes[i]][atende[vetor_clientes[i]]]*d[vetor_clientes[i]] ;
    s[atende[vetor_clientes[i]]].remove(vetor_clientes[i]);
    s[*k3].push_back(vetor_clientes[i]);
    pen[atende[vetor_clientes[i]]] = pen[atende[vetor_clientes[i]]] - d[vetor_clientes[i]];
    pen[*k3] = pen[*k3] + d[vetor_clientes[i]];
    atende[vetor_clientes[i]] = *k3;
    //cout << "faz uma realocacao" << endl;
    return;
    


}
}
}
}


}

void explora_vizinhanca_Movement2troca_restrito_factivel_first_improvement (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis)
{

for (int i = 0; i < vetor_clientes.size(); i++)
{
		int r = rand() % vetor_clientes.size();

		int temp = vetor_clientes[i];
		vetor_clientes[i] = vetor_clientes[r];
		vetor_clientes[r] = temp;
}

//vector<int> clientes_de_melhora1;
//vector<int> clientes_de_melhora2;
//vector<double> melhoras;

//explora toda a vizinhança
//cout << "começa a explorar a vizinhnaça:" << endl;
for (int i1 = 0; i1 < vetor_clientes.size() - 1; i1++){   //para cada cliente
for (int i2 = i1 + 1; i2 < vetor_clientes.size() ; i2++){    //para cada cliente

double penalidade_recebe1 = pen[atende[vetor_clientes[i2]]] + d[vetor_clientes[i1]] - d[vetor_clientes[i2]];
double penalidade_recebe2 = pen[atende[vetor_clientes[i1]]] + d[vetor_clientes[i2]] - d[vetor_clientes[i1]];

if (  (penalidade_recebe1 <= 0) && (penalidade_recebe2 <= 0) && (atende[vetor_clientes[i1]] != atende[vetor_clientes[i2]])  )
{

double antes = c[vetor_clientes[i1]][atende[vetor_clientes[i1]]]*d[vetor_clientes[i1]] + c[vetor_clientes[i2]][atende[vetor_clientes[i2]]]*d[vetor_clientes[i2]] ;
double depois =  c[vetor_clientes[i1]][atende[vetor_clientes[i2]]]*d[vetor_clientes[i1]] + c[vetor_clientes[i2]][atende[vetor_clientes[i1]]]*d[vetor_clientes[i2]] ;


if (depois < antes)
{

//faz o movimento e retorna;
fo = fo + depois - antes;
   
    s[atende[vetor_clientes[i1]]].remove(vetor_clientes[i1]);
    s[atende[vetor_clientes[i2]]].remove(vetor_clientes[i2]);
    s[atende[vetor_clientes[i2]]].push_back(vetor_clientes[i1]);
    s[atende[vetor_clientes[i1]]].push_back(vetor_clientes[i2]);
    pen[atende[vetor_clientes[i1]]] = penalidade_recebe2;
    pen[atende[vetor_clientes[i2]]] = penalidade_recebe1;
    int temp = atende[vetor_clientes[i1]];
    atende[vetor_clientes[i1]] = atende[vetor_clientes[i2]];
    atende[vetor_clientes[i2]] = temp;
    return;


}



}
} // fim for
} // fim for


}

void explora_vizinhanca_movimento_fecha2_first_improvement(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p, double menor_demanda)
{

double soma_das_capacidades_abertas = 0;
for (std::list<int>::iterator k2 = abertas.begin(); k2 != abertas.end(); k2++ ){
soma_das_capacidades_abertas = soma_das_capacidades_abertas + p[*k2];
}

//cout << "tamanho fechadas" << fechadas.size() << endl;
//cout << "tamanho abertas" << abertas.size() << endl;

vector<double> pen_copia(m);
for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
}
list<int> abertas_copia;
list<int> fechadas_copia;
vector<int> atende_copia(n);
cria_copia_vetor_int(atende_copia,atende);
cria_copia_lista(abertas_copia, abertas);
cria_copia_lista(fechadas_copia, fechadas);
bool conseguiu_alocar = false;
double fo_copia;
bool houve_melhora = false;
vector<int> auxiliar;
for (std::list<int>::iterator k1 = abertas.begin(); k1 != abertas.end(); k1++ ){

 auxiliar.push_back(*k1);
}

for (int i = 0; i < auxiliar.size(); i++)
{
		int r = rand() % auxiliar.size();

		int temp = auxiliar[i];
		auxiliar[i] = auxiliar[r];
		auxiliar[r] = temp;
}



abertas.clear();
for (int i = 0; i < auxiliar.size(); i++)
{
   abertas.push_back(auxiliar[i]);
}




for (std::list<int>::iterator k1 = abertas.begin(); k1 != abertas.end(); k1++ ){

    
    fo_copia = fo;
    //cout << "tenta fechar: " << *k1 << "de custo de abertura: " << f[*k1] << endl;
    abertas_copia.remove(*k1);
    fechadas_copia.push_back(*k1);
    fo_copia = fo_copia - f[*k1];
    vector<int> vetor_clientes_fecha;
    double soma_das_demandas_clientes_fecha = 0;


    for ( std::list<int>::iterator k = s[*k1].begin(); k != s[*k1].end(); k++)
    {
      vetor_clientes_fecha.push_back(*k);
     
      soma_das_demandas_clientes_fecha = soma_das_demandas_clientes_fecha + d[*k];
            //aloca cada cliente em outras facilidades abertas
    }

        if(  (soma_das_capacidades_abertas - p[*k1]) < soma_das_demandas_clientes_fecha)
    {
        //cout << "capacidade total com o fechamento insuficiente para atender os clientes" << endl;
        abertas_copia.push_back(*k1); // tenta fechar a proxima
        fechadas_copia.remove(*k1);
        fo_copia = fo_copia + f[*k1];
        continue;
    }
    //vector<int> atende_depois(vetor_clientes_fecha.size());
    ordena_clientes_decrescente_demanda(vetor_clientes_fecha, d); 
        //cout << "vetor clientes fecha" << endl;
      /*  
    for (int i = 0; i < vetor_clientes_fecha.size(); i++)
    {
       cout << vetor_clientes_fecha[i] << " (" << d[vetor_clientes_fecha[i]] << " )"; 
    }
    cout << endl;
    */
   
    vector<int> vetor_abertas_possiveis;
    for (std::list<int>::iterator k2 = abertas_copia.begin(); k2 != abertas_copia.end(); k2++ ){
      if (  (pen_copia[*k2] + menor_demanda <= 0)   )
      {
      vetor_abertas_possiveis.push_back(*k2);
      }
      }


    if( vetor_abertas_possiveis.size() == 0)
    {
    //cout << "nao ha facilidade disponivel para atender a menor demanda" << endl;
    abertas_copia.push_back(*k1); // tenta fechar a proxima
    fechadas_copia.remove(*k1);
    fo_copia = fo_copia + f[*k1];
    continue; // tenta fechar a proxima
    }

    //para cada clientes     


    conseguiu_alocar = false;
    for (int i = 0; i < vetor_clientes_fecha.size(); i++)
    {
        //cout << "tenta alocar cliente: " << vetor_clientes_fecha[i] << endl;
    //seleciona as que cabem
    conseguiu_alocar = false;
    int indice_primeiro_que_cabe = -1;
    for (int j = 0; j < vetor_abertas_possiveis.size(); j++)
    {
        if (pen_copia[vetor_abertas_possiveis[j]] + d[vetor_clientes_fecha[i]] <= 0 )
        {
             indice_primeiro_que_cabe = j;
             conseguiu_alocar = true;
             break; 
        }
    }
    if (indice_primeiro_que_cabe == -1)
    {
       conseguiu_alocar = false;     
       //cout << "nao conseguiu aloca certo cliente" << endl;
       break; // sai do para cada clientes
    }
    if (conseguiu_alocar == true)
    {
          // cout << " conseguiu alocar " << endl;
         int indice_que_cabe_de_menor_custo = indice_primeiro_que_cabe;
        // cout << "indice primeiro que cabe" << indice_que_cabe_de_menor_custo << endl;
         for (int j = indice_primeiro_que_cabe; j < vetor_abertas_possiveis.size() ; j++)
         {
            if (   (pen_copia[vetor_abertas_possiveis[j]] + d[vetor_clientes_fecha[i]] <= 0 ) && ( c[vetor_clientes_fecha[i]][vetor_abertas_possiveis[j]] < c[vetor_clientes_fecha[i]][vetor_abertas_possiveis[indice_que_cabe_de_menor_custo]] ) )
            {
            indice_que_cabe_de_menor_custo = j;
           // cout << "atualiza indice melhor que cabe: " << indice_que_cabe_de_menor_custo << endl;
            }
         }

        // cout << "termino processo de encontrar a melhor que cabe: " << endl;
      fo_copia = fo_copia - c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]] ;
      pen_copia[atende_copia[vetor_clientes_fecha[i]]] = pen_copia[atende_copia[vetor_clientes_fecha[i]]]  - d[vetor_clientes_fecha[i]];
      atende_copia[vetor_clientes_fecha[i]] = vetor_abertas_possiveis[indice_que_cabe_de_menor_custo];
      pen_copia[atende_copia[vetor_clientes_fecha[i]]] =  pen_copia[atende_copia[vetor_clientes_fecha[i]]] + d[vetor_clientes_fecha[i]];
      fo_copia = fo_copia + c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]] ;  
     //cout << "conseguiu alocar" << vetor_clientes_fecha[i] << endl;
    }
    }
    /*
    if (conseguiu_alocar == true)
    {
        //cout << "conseguiu alocar todos os clinetes com fo:" << fo_copia << endl;   
        //cout << "confere solucao:" << endl;      
        confere_solucao_atende (m, n,c,  f, atende_copia, d);
        cout << endl;


    }
    */

   
    if ( (conseguiu_alocar == true) && (fo_copia < fo) )
    {
    houve_melhora = true;  
    //cout << endl;  
    fo = fo_copia;
     //cout << "conseguiu alocar todos os clientes e houve melhora:" << fo << endl;  
    
    for (int i = 0; i < vetor_clientes_fecha.size(); i++)
    {
        s[atende[vetor_clientes_fecha[i]]].remove(vetor_clientes_fecha[i]);
        s[atende_copia[vetor_clientes_fecha[i]]].push_back(vetor_clientes_fecha[i]);
        pen[atende[vetor_clientes_fecha[i]]] = pen_copia[atende[vetor_clientes_fecha[i]]];
        pen[atende_copia[vetor_clientes_fecha[i]]] = pen_copia[atende_copia[vetor_clientes_fecha[i]]];
        atende[vetor_clientes_fecha[i]] = atende_copia[vetor_clientes_fecha[i]];
        
       
        //cout << "abertas" <<  abertas.size() << endl;
        //cout << "fechadas" <<  abertas.size() << endl;
        //cout << "fechadas" <<  abertas.size() << endl;



    }
         abertas.remove(*k1);
        fechadas.push_back(*k1);
        //cout << "abertas:" <<  abertas.size() << endl;
        //cout << "fechadas:" <<  fechadas.size() << endl;
          //      confere_solucao_s( m, n, c,  f, s, d, abertas, fechadas,pen,  p );
        //cout << endl;
        
    }
    else
    {
        //cout << "desfaz movimento" << endl;
        //volta tudo que era antes
        //abertas_copia.push_back(*k1);
        fo_copia = fo;
        
            for (int i = 0; i < vetor_clientes_fecha.size(); i++)
         {
        //fo_copia = fo_copia + c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]]  ;   
         //fo_copia = fo_copia + c[vetor_clientes_fecha[i]][atende[vetor_clientes_fecha[i]]] - c[vetor_clientes_fecha[i]][atende_depois[vetor_clientes_fecha[i]]];    
        //s[atende[vetor_clientes_fecha[i]]].push_back(vetor_clientes_fecha[i]);
        //s[atende_copia[vetor_clientes_fecha[i]]].push_back(vetor_clientes_fecha[i]);
        pen_copia[atende[vetor_clientes_fecha[i]]] = pen[atende[vetor_clientes_fecha[i]]];
        pen_copia[atende_copia[vetor_clientes_fecha[i]]] = pen[atende_copia[vetor_clientes_fecha[i]]];
        atende_copia[vetor_clientes_fecha[i]] = atende[vetor_clientes_fecha[i]];
        //fo_copia = fo_copia - c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]]; 
        abertas_copia.push_back(*k1);
        fechadas_copia.remove(*k1);


         }

        // cout << "fo_desfeito: " << fo_copia << endl;
          //      cout << "confere fo desfeito:" << fo_copia << endl; 
        
        //confere_solucao_atende (m, n,c,  f,atende_copia, d);




    }


    if (houve_melhora == true)
    {
          //  cout << "confere solucao s" << endl;
          // confere_solucao_s( m, n, c,  f, s, d, abertas, fechadas,pen,  p );
          //exit(0);
        return;
    }



      //aloca cliente na facilidade de menor custo.




    }


    //cout << "confere solucao s" << endl;
    //confere_solucao_s( m, n, c,  f, s, d, abertas, fechadas,pen,  p );

    //exit(0);

    } // void

void explora_vizinhanca_movimento_fecha2_best_improvement(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p, double menor_demanda)
{
/*
double soma_das_capacidades_abertas = 0;
for (std::list<int>::iterator k2 = abertas.begin(); k2 != abertas.end(); k2++ ){
soma_das_capacidades_abertas = soma_das_capacidades_abertas + p[*k2];
}
*/
//cout << "tamanho fechadas" << fechadas.size() << endl;
//cout << "tamanho abertas" << abertas.size() << endl;
vector<double> pen_copia(m);
list<int> abertas_copia;
list<int> fechadas_copia;
vector<int> atende_copia(n);
//cria_copia_vetor_int(atende_copia,atende);
//cria_copia_lista(abertas_copia, abertas);
//cria_copia_lista(fechadas_copia, fechadas);
bool conseguiu_alocar = false;
double fo_copia;
bool houve_melhora = false;
int melhor_abre;
vector<int> atende_melhor(n);
double fo_melhor = fo;
bool pare = false;



/*
for (std::list<int>::iterator k1 = abertas.begin(); k1 != abertas.end(); k1++ ){

 auxiliar.push_back(*k1);
}

for (int i = 0; i < auxiliar.size(); i++)
{
		int r = rand() % auxiliar.size();

		int temp = auxiliar[i];
		auxiliar[i] = auxiliar[r];
		auxiliar[r] = temp;
}

*/
/*
abertas.clear();
for (int i = 0; i < auxiliar.size(); i++)
{
   abertas.push_back(auxiliar[i]);
}
*/
 double soma_das_capacidades_abertas;
while (pare == false)
{
    soma_das_capacidades_abertas = 0;
    for (std::list<int>::iterator k2 = abertas.begin(); k2 != abertas.end(); k2++ ){
        soma_das_capacidades_abertas = soma_das_capacidades_abertas + p[*k2];
    }
houve_melhora = false;
//cout << "tamanho fechadas" << fechadas.size() << endl;
//cout << "tamanho abertas" << abertas.size() << endl;


for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
}
cria_copia_vetor_int(atende_copia,atende);
cria_copia_lista(abertas_copia, abertas);
cria_copia_lista(fechadas_copia, fechadas);
conseguiu_alocar = false;






for (std::list<int>::iterator k1 = abertas.begin(); k1 != abertas.end(); k1++ ){

    
    fo_copia = fo;
    //cout << "tenta fechar: " << *k1 << "de custo de abertura: " << f[*k1] << endl;
    abertas_copia.remove(*k1);
    fechadas_copia.push_back(*k1);
    fo_copia = fo_copia - f[*k1];
    vector<int> vetor_clientes_fecha;
    double soma_das_demandas_clientes_fecha = 0;


    for ( std::list<int>::iterator k = s[*k1].begin(); k != s[*k1].end(); k++)
    {
      vetor_clientes_fecha.push_back(*k);
     
      soma_das_demandas_clientes_fecha = soma_das_demandas_clientes_fecha + d[*k];
            //aloca cada cliente em outras facilidades abertas
    }

        if(  (soma_das_capacidades_abertas - p[*k1]) < soma_das_demandas_clientes_fecha)
    {
        //cout << "capacidade total com o fechamento insuficiente para atender os clientes" << endl;
        abertas_copia.push_back(*k1); // tenta fechar a proxima
        fechadas_copia.remove(*k1);
        fo_copia = fo_copia + f[*k1];
        continue;
    }
    //vector<int> atende_depois(vetor_clientes_fecha.size());
    ordena_clientes_decrescente_demanda(vetor_clientes_fecha, d); 
        //cout << "vetor clientes fecha" << endl;
      /*  
    for (int i = 0; i < vetor_clientes_fecha.size(); i++)
    {
       cout << vetor_clientes_fecha[i] << " (" << d[vetor_clientes_fecha[i]] << " )"; 
    }
    cout << endl;
    */
   
    vector<int> vetor_abertas_possiveis;
    for (std::list<int>::iterator k2 = abertas_copia.begin(); k2 != abertas_copia.end(); k2++ ){
      if (  (pen_copia[*k2] + menor_demanda <= 0)   )
      {
      vetor_abertas_possiveis.push_back(*k2);
      }
      }


    if( vetor_abertas_possiveis.size() == 0)
    {
    //cout << "nao ha facilidade disponivel para atender a menor demanda" << endl;
    abertas_copia.push_back(*k1); // tenta fechar a proxima
    fechadas_copia.remove(*k1);
    fo_copia = fo_copia + f[*k1];
    continue; // tenta fechar a proxima
    }

    //para cada clientes     


    conseguiu_alocar = false;
    for (int i = 0; i < vetor_clientes_fecha.size(); i++)
    {
        //cout << "tenta alocar cliente: " << vetor_clientes_fecha[i] << endl;
    //seleciona as que cabem
    conseguiu_alocar = false;
    int indice_primeiro_que_cabe = -1;
    for (int j = 0; j < vetor_abertas_possiveis.size(); j++)
    {
        if (pen_copia[vetor_abertas_possiveis[j]] + d[vetor_clientes_fecha[i]] <= 0 )
        {
             indice_primeiro_que_cabe = j;
             conseguiu_alocar = true;
             break; 
        }
    }
    if (indice_primeiro_que_cabe == -1)
    {
       conseguiu_alocar = false;     
       //cout << "nao conseguiu aloca certo cliente" << endl;
       break; // sai do para cada clientes
    }
    if (conseguiu_alocar == true)
    {
          // cout << " conseguiu alocar " << endl;
         int indice_que_cabe_de_menor_custo = indice_primeiro_que_cabe;
        // cout << "indice primeiro que cabe" << indice_que_cabe_de_menor_custo << endl;
         for (int j = indice_primeiro_que_cabe; j < vetor_abertas_possiveis.size() ; j++)
         {
            if (   (pen_copia[vetor_abertas_possiveis[j]] + d[vetor_clientes_fecha[i]] <= 0 ) && ( c[vetor_clientes_fecha[i]][vetor_abertas_possiveis[j]] < c[vetor_clientes_fecha[i]][vetor_abertas_possiveis[indice_que_cabe_de_menor_custo]] ) )
            {
            indice_que_cabe_de_menor_custo = j;
           // cout << "atualiza indice melhor que cabe: " << indice_que_cabe_de_menor_custo << endl;
            }
         }

        // cout << "termino processo de encontrar a melhor que cabe: " << endl;
      fo_copia = fo_copia - c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]] ;
      pen_copia[atende_copia[vetor_clientes_fecha[i]]] = pen_copia[atende_copia[vetor_clientes_fecha[i]]]  - d[vetor_clientes_fecha[i]];
      atende_copia[vetor_clientes_fecha[i]] = vetor_abertas_possiveis[indice_que_cabe_de_menor_custo];
      pen_copia[atende_copia[vetor_clientes_fecha[i]]] =  pen_copia[atende_copia[vetor_clientes_fecha[i]]] + d[vetor_clientes_fecha[i]];
      fo_copia = fo_copia + c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]] ;  
     //cout << "conseguiu alocar" << vetor_clientes_fecha[i] << endl;
    }
    }
    /*
    if (conseguiu_alocar == true)
    {
        //cout << "conseguiu alocar todos os clinetes com fo:" << fo_copia << endl;   
        //cout << "confere solucao:" << endl;      
        confere_solucao_atende (m, n,c,  f, atende_copia, d);
        cout << endl;


    }
    */

   
    if ( (conseguiu_alocar == true) && (fo_copia < fo_melhor) )
    {
    //cout << "houve melhora:" << endl;
    
    houve_melhora = true;  
    fo_melhor = fo_copia;
    //cout << fo_melhor << endl;
    melhor_abre = *k1;
    for (int i = 0; i < n; i++)
    {
       atende_melhor[i] = atende_copia[i];
    }

    }

    //volta tudo o que era antes
    
        //cout << "desfaz movimento" << endl;
        //volta tudo que era antes
        //abertas_copia.push_back(*k1);
        fo_copia = fo;
        
            for (int i = 0; i < vetor_clientes_fecha.size(); i++)
         {
        //fo_copia = fo_copia + c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]]  ;   
         //fo_copia = fo_copia + c[vetor_clientes_fecha[i]][atende[vetor_clientes_fecha[i]]] - c[vetor_clientes_fecha[i]][atende_depois[vetor_clientes_fecha[i]]];    
        //s[atende[vetor_clientes_fecha[i]]].push_back(vetor_clientes_fecha[i]);
        //s[atende_copia[vetor_clientes_fecha[i]]].push_back(vetor_clientes_fecha[i]);
        pen_copia[atende[vetor_clientes_fecha[i]]] = pen[atende[vetor_clientes_fecha[i]]];
        pen_copia[atende_copia[vetor_clientes_fecha[i]]] = pen[atende_copia[vetor_clientes_fecha[i]]];
        atende_copia[vetor_clientes_fecha[i]] = atende[vetor_clientes_fecha[i]];
        //fo_copia = fo_copia - c[vetor_clientes_fecha[i]][atende_copia[vetor_clientes_fecha[i]]]*d[vetor_clientes_fecha[i]]; 
        abertas_copia.push_back(*k1);
        fechadas_copia.remove(*k1);
         }
     }

    //cout << "terminou de explorar vizinhanca" << endl;
    
    
    
     if (houve_melhora == true)  //faz o movimento
     {
                abertas.clear();
                fechadas.clear();
                for (int j = 0; j < m; j++)
                {
                    s[j].clear();
                }
                for (int j = 0; j < m; j++)
                {
                   pen[j] = -p[j];
                }
                
                for (int i = 0; i < n; i++)
                {
                  atende[i] = atende_melhor[i];
                  s[atende[i]].push_back(i);
                  pen[atende[i]] = pen[atende[i]] + d[i];
                   
                }
                for (int j = 0; j < m; j++)
                {
                   if (! s[j].empty() )
                   {
                    abertas.push_back(j);
                   }
                   else
                   {
                    fechadas.push_back(j);
                   }
                }
                fo = fo_melhor;
                


     }    
     else
     {
            pare = true;
     }




}
         


    } // void



 void VND_factivel_1_2_3(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis, vector<double> p, double menor_demanda) 
{
//cout << "começa vnd ________________________________________________________" << endl;    

double fo_antes = fo;
bool pare = false;
int vizinhanca = 1;

int iter = 0;
while (pare == false)
{

if (vizinhanca == 1)
{
explora_vizinhanca_Movement1_restrito_factivel_first_improvement(m,  n, s ,fo, pen,atende, c, d,f, abertas, fechadas, vetor_clientes , factivel,abertas_disponiveis);

//explora_vizinhanca_Movement1_restrito_factivel(m,n, s ,fo,pen, atende, c,d, f, abertas, fechadas, vetor_clientes, factivel, abertas_disponiveis);
//cout << "movimento 1: fo = " << fo << endl;
//cout << "confere solucao:" << endl;
//confere_solucao_s( m, n,  c,f,s,d,abertas, fechadas,  pen,  p );
//cout << "confere apos movimento 1:" << endl;
//cout << "movimento 1: fo = " << fo << " " << endl;
//cout << "confere" << endl;
//confere_solucao_s( m, n, c,f, s,  d, abertas, fechadas,pen, p);
//cout << endl;

//cout << endl;
}
if (vizinhanca == 2)
{

/*
    int soma_clientes_alocados = 0;
    for (int i = 0; i < n; i++)
{
    cout << i <<" ("<< atende[i] << ") ";
}
cout << endl;

for (int j = 0; j < m; j++)
{
   if (!s[j].empty())
   {
    cout << j << ": ";
    for (std::list<int>::iterator k1 = s[j].begin(); k1 != s[j].end(); k1++ ){
        soma_clientes_alocados = soma_clientes_alocados +1;
    cout << *k1 << " ";
    }
    cout << endl;
   }

}
cout << "quantidade clientes alocados" << soma_clientes_alocados << endl;
*/

explora_vizinhanca_Movement2troca_restrito_factivel_first_improvement (m,n, s ,fo,pen, atende, c,d, f, abertas, fechadas, vetor_clientes, factivel, abertas_disponiveis);

//cout << "movimento 2: fo = " << fo << endl;
//cout << "confere solucao:" << endl;
//confere_solucao_s( m, n,  c,f,s,d,abertas, fechadas,  pen,  p );
//cout << endl;
//explora_vizinhanca_Movement2troca_restrito_factivel(m,n, s ,fo,pen, atende, c,d, f, abertas, fechadas, vetor_clientes, factivel, abertas_disponiveis);
//cout << "movimento 2: fo = " << fo << " ";
//confere_solucao_atende(m,n,c,f,atende,d);
//confere_solucao_s( m, n, c,f, s,  d, abertas, fechadas,pen, p);
//cout << endl;
//cout << "confere apos movimento 2:" << endl;
//cout << "movimento 2: fo = " << fo << " " << endl;
//cout << "confere" << endl;
//confere_solucao_s( m, n, c,f, s,  d, abertas, fechadas,pen, p);
//cout << endl;
//exit(0);
}

/*
if (vizinhanca == 3)
{

explora_vizinhanca_Movement2realoca2_restrito ( m, n,s ,fo,pen, atende,c,d, f, abertas, fechadas, vetor_clientes ,factivel, abertas_disponiveis);

}
*/



if (vizinhanca == 3)
{
explora_vizinhanca_movimento_fecha2_best_improvement( m, n, s ,fo, pen, atende, c, d, f, abertas, fechadas, p,menor_demanda);
//explora_vizinhanca_movimento_fecha2_first_improvement( m, n,s , fo, pen,atende,  c, d, f, abertas, fechadas, p, menor_demanda);

//cout << "fo apos movimento = " << fo << " " << endl;
abertas_disponiveis.clear();
cria_copia_lista(abertas_disponiveis, abertas);
//cria_copia_lista(fechadas_disponiveis, fechadas);

//for (std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++ ){
//abertas_disponiveis.push_back(*k);
//cout << "confere apos movimento fecha:" << endl;
//confere_solucao_s( m, n, c,f, s,  d, abertas, fechadas,pen, p);
//cout << endl;

//exit(0);
//}



//cout << endl;


}


if (fo < fo_antes)
{
    vizinhanca = 1;
    fo_antes = fo;
}
else
{
    vizinhanca = vizinhanca  + 1;
}

if (vizinhanca == 4) //numero de vizinhancas + 1
{
    pare = true;
}



}

} // fim void vnd

void VND_factivel_1_2_3_ils(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis, vector<double> p) 
{

double fo_antes = fo;
bool pare = false;
int vizinhanca = 1;

int iter = 0;
while (pare == false)
{

if (vizinhanca == 1)
{
explora_vizinhanca_Movement1_restrito_factivel_first_improvement(m,  n, s ,fo, pen,atende, c, d,f, abertas, fechadas, vetor_clientes , factivel,abertas_disponiveis);
//cout << "movimento 1: fo = " << fo << " " << endl;
//confere_solucao_atende(m,n,c,f,atende,d);
//confere_solucao_s( m, n, c,f, s,  d, abertas, fechadas,pen, p);
//cout << endl;
}
if (vizinhanca == 2)
{

/*
    int soma_clientes_alocados = 0;
    for (int i = 0; i < n; i++)
{
    cout << i <<" ("<< atende[i] << ") ";
}
cout << endl;

for (int j = 0; j < m; j++)
{
   if (!s[j].empty())
   {
    cout << j << ": ";
    for (std::list<int>::iterator k1 = s[j].begin(); k1 != s[j].end(); k1++ ){
        soma_clientes_alocados = soma_clientes_alocados +1;
    cout << *k1 << " ";
    }
    cout << endl;
   }

}
cout << "quantidade clientes alocados" << soma_clientes_alocados << endl;
*/

explora_vizinhanca_Movement2troca_restrito_factivel_first_improvement (m,n, s ,fo,pen, atende, c,d, f, abertas, fechadas, vetor_clientes, factivel, abertas_disponiveis);

//explora_vizinhanca_Movement2troca_restrito_factivel(m,n, s ,fo,pen, atende, c,d, f, abertas, fechadas, vetor_clientes, factivel, abertas_disponiveis);
//cout << "movimento 2: fo = " << fo << " ";
//confere_solucao_atende(m,n,c,f,atende,d);
//confere_solucao_s( m, n, c,f, s,  d, abertas, fechadas,pen, p);
//cout << endl;
}
/*
if (vizinhanca == 3)
{
explora_vizinhanca_Movement2realoca2_restrito ( m, n,s ,fo,pen, atende,c,d, f, abertas, fechadas, vetor_clientes ,factivel, abertas_disponiveis);

}
*/


if (fo < fo_antes)
{
    vizinhanca = 1;
    fo_antes = fo;
}
else
{
    vizinhanca = vizinhanca  + 1;
}

if (vizinhanca == 3) //numero de vizinhancas + 1
{
    pare = true;
}



}

} // fim void vnd

void ILS_1_novo(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>&facilidades_potenciais, vector<double> p, int tamanho_instancia, vector<vector<int>> matriz_clientes_ordenados, double menor_demanda , double soma_das_demandas, int ils_max1)
{
//cout << "COMEÇA ILS-I NOVO ____________________________________________________________________" << endl;
//cout << "abertas:" << endl;
//imprime_lista(abertas);
//cout << endl;
//cout << "fechadas:" << endl;
//imprime_lista(fechadas);
//cout << endl;
//cout << "valor do fo no inicio: " << fo << endl;

//int vezesmax = ILSmax; 
//list<int> locais_proibidos;
vector<list<int>> s_copia(m);
vector<int> atende_copia(n);
vector<double> pen_copia(m);
list<int> abertas_copia;
list<int> fechadas_copia;
double fo_copia;
// criar s_copia abertas_copia e fechadas_copia
for (int j = 0; j < m; j++){
if (! s[j].empty() )
{
abertas_copia.push_back(j);

for ( std::list<int>::iterator  k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
// criar fo copia e atende copia
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
fo_copia = fo;


int iter = 0;



vector<int> vetor_fechadas;
for ( std::list<int>::iterator k = fechadas_copia.begin(); k != fechadas_copia.end(); k++ ){

    vetor_fechadas.push_back(*k);
}
for (int i = 0; i < vetor_fechadas.size(); i++)
{
		int r = rand() % vetor_fechadas.size();

		int temp = vetor_fechadas[i];
		vetor_fechadas[i] = vetor_fechadas[r];
		vetor_fechadas[r] = temp;
}
int auxx = vetor_fechadas.size();
ils_max1 = min(ils_max1, auxx);
//cout << "ils max 1 = " << ils_max1 << endl;
int k = 0;
int s1;
vector<int> vetor_clientes(n);
for (int i = 0; i < n; i++)
{
    vetor_clientes[i] = i;
}
while(iter < ils_max1)
{

double fo1 = 0;
double fo2 = 0;






for ( std::list<int>::iterator k = abertas_copia.begin(); k != abertas_copia.end(); k++ ){

    fo1 = fo1 + f[*k];
}



//cout << "custos fixos abertura antes perturbacao:" << fo1 << endl;

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < m; j++)
    {
        if (atende_copia[i] == j)
        {
            fo2 = fo2 + c[i][j]*d[i];
        }
    }
    
}

s1 = vetor_fechadas[k];

//cout << "custos atendimento antes perturbacao:" << fo2 << endl;


//cout << "confere solucao s antes de fazer a perturbacao:" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;   

//sorteia duas facilidades fechadas
/*
int s1 = rand() % m;

while (esta_na_lista(abertas_copia,s1) || esta_na_lista(locais_proibidos,s1))
{
    s1 = rand() % m;
}
locais_proibidos.push_back(s1);
*/


/*
int s2 = rand() % m;
while (esta_na_lista(abertas_copia,s2) || (s2 == s1))
{
    s2 = rand() % m;
}
*/
/*
int s3 = rand() % m;
while (esta_na_lista(abertas_copia,s3) || (s3 == s1) || (s3 == s2) )
{
    s3 = rand() % m;
}
*/

//cout << "facilidades abertas: " << s1 <<  endl;
abertas_copia.push_back(s1);
//abertas_copia.push_back(s2);
//abertas_copia.push_back(s3);

//fechadas_copia.remove(s3);
fechadas_copia.remove(s1);
//fechadas_copia.remove(s2);
fo1 = fo1 + f[s1];
//fo1 = fo1 + f[s1] + f[s2]
//fo1 = fo1 + f[s1] + f[s2] + f[s3];
//fo_copia = fo_copia + f[s1] + f[s2];


//realoca para s1 e s2 clientes mais próximos até que a capacidade esgote
int i = 0;
int conta1 = 0;
int conta2 = 0;
while ( i < n )
{
    if ( (pen_copia[s1] + d[matriz_clientes_ordenados[s1][i]] <= 0) && (s_copia[atende_copia[matriz_clientes_ordenados[s1][i]]].size() > 1 )   )
    {
        
        s_copia[atende_copia[matriz_clientes_ordenados[s1][i]]].remove(matriz_clientes_ordenados[s1][i]);
        pen_copia[atende_copia[matriz_clientes_ordenados[s1][i]]] = pen_copia[atende_copia[matriz_clientes_ordenados[s1][i]]] - d[matriz_clientes_ordenados[s1][i]];
        pen_copia[s1] = pen_copia[s1] + d[matriz_clientes_ordenados[s1][i]];
        s_copia[s1].push_back(matriz_clientes_ordenados[s1][i]);
        fo2 = fo2 - c[matriz_clientes_ordenados[s1][i]][atende_copia[matriz_clientes_ordenados[s1][i]]]*d[matriz_clientes_ordenados[s1][i]] ;
        fo2 = fo2 + c[matriz_clientes_ordenados[s1][i]][s1]*d[matriz_clientes_ordenados[s1][i]] ;
        atende_copia[matriz_clientes_ordenados[s1][i]] = s1;
        conta1 = conta1 + 1;
    }
      i = i + 1;
      if ((abs(pen_copia[s1])) < menor_demanda)
      {
        i = n;
      }

}
/*
 i = 0;

while ( i < n )
{
    //cout << i << endl;
    if ( (pen_copia[s2] + d[matriz_clientes_ordenados[s2][i]] <= 0) &&  (s_copia[atende_copia[matriz_clientes_ordenados[s2][i]]].size() > 1 ) )
    {
        //realoca
        s_copia[atende_copia[matriz_clientes_ordenados[s2][i]]].remove(matriz_clientes_ordenados[s2][i]);
        pen_copia[s2] = pen_copia[s2] + d[matriz_clientes_ordenados[s2][i]];
        s_copia[s2].push_back(matriz_clientes_ordenados[s2][i]);
        fo2 = fo2 - c[matriz_clientes_ordenados[s2][i]][atende_copia[matriz_clientes_ordenados[s2][i]]]*d[matriz_clientes_ordenados[s2][i]];
        fo2 = fo2 + c[matriz_clientes_ordenados[s2][i]][s2]*d[matriz_clientes_ordenados[s2][i]];
        atende_copia[matriz_clientes_ordenados[s2][i]] = s2;
        conta2 = conta2 + 1;
    }
     i = i + 1;
         if (pen_copia[s2] == 0)
      {
        i = n;
      }
}
*/

 //i = 0;
/*
while ( i < n )
{
    //cout << i << endl;
    if ( (pen_copia[s3] + d[matriz_clientes_ordenados[s3][i]] <= 0) &&  (s_copia[atende_copia[matriz_clientes_ordenados[s3][i]]].size() > 1 ) )
    {
        //realoca
        s_copia[atende_copia[matriz_clientes_ordenados[s3][i]]].remove(matriz_clientes_ordenados[s3][i]);
        pen_copia[s3] = pen_copia[s3] + d[matriz_clientes_ordenados[s3][i]];
        s_copia[s3].push_back(matriz_clientes_ordenados[s3][i]);
        fo2 = fo2 - c[matriz_clientes_ordenados[s3][i]][atende_copia[matriz_clientes_ordenados[s3][i]]]*d[matriz_clientes_ordenados[s3][i]];
        fo2 = fo2 + c[matriz_clientes_ordenados[s3][i]][s3]*d[matriz_clientes_ordenados[s3][i]];
        atende_copia[matriz_clientes_ordenados[s3][i]] = s3;
        conta2 = conta2 + 1;
    }
     i = i + 1;
         if (pen_copia[s3] == 0)
      {
        i = n;
      }
}
*/

fo_copia = fo1 + fo2;
//cout << endl;
//cout << "fo apos perturbacao: " << fo_copia << endl;
//cout << "confere solucao s" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;
//cout << "penalidade primeira: " << pen_copia[s1] << endl;
//cout << "penalidade segunda: " << pen_copia[s2] << endl;




/*
 fo1 = 0;
 fo2 = 0;

for ( std::list<int>::iterator k = abertas_copia.begin(); k != abertas_copia.end(); k++ )
{
    fo1 = fo1 + f[*k];
}

cout << "custos fixos abertura depois perturbacao:" << fo1 << endl;

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < m; j++)
    {
        if (atende_copia[i] == j)
        {
            fo2 = fo2 + c[i][j]*d[i];
        }
    }
    
}
*/


//cout << "custos atendimento depois  perturbacao:" << fo2 << endl;


//cout << "clientes realocados em : " << s1 << ":" << conta1 << endl;
//cout << "clientes realocados em : " << s2 << ":" << conta2 << endl;






//confere_solucao_atende (m, n, c,f,  atende_copia,d);
//cout << endl;


//cout << "_________________________________________________________________________________" << endl;

bool factivel = true;
//list<int> 
VND_factivel_1_2_3( m, n, s_copia , fo_copia, pen_copia, atende_copia, c,d,f, abertas_copia, fechadas_copia, vetor_clientes, factivel,  abertas_copia, p, menor_demanda) ;

//cout << "fo apos busca local: " << fo_copia << endl;
//cout << "confere solucao" << endl;
//confere_solucao_atende (m, n, c,f,  atende_copia,d);
//cout << endl;
//cout << "confere solucao s" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;
//exit(0);
//cout << "__________________________________________________________________________________" << endl;
if (fo_copia < 0.99999999999*fo) //houve melhora fica com a solucao
{


//locais_proibidos.clear();
abertas.clear();
fechadas.clear();
for (int j = 0; j < m; j++){
s[j].clear();
if (! s_copia[j].empty() )
{
abertas.push_back(j);
for ( std::list<int>::iterator k = s_copia[j].begin(); k != s_copia[j].end(); k++ ){
s[j].push_back(*k); 
}
}
else
{
fechadas.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen[j] = pen_copia[j];
} 
for (int i = 0; i < n; i++)
{
    atende[i] = atende_copia[i];
} 
fo = fo_copia;
iter = 0;
//cout << "fo melhor: " << fo << endl;
k = 0;
vetor_fechadas.clear();
for ( std::list<int>::iterator k = fechadas.begin(); k != fechadas.end(); k++ ){

    vetor_fechadas.push_back(*k);
}
for (int i = 0; i < vetor_fechadas.size(); i++)
{
		int r = rand() % vetor_fechadas.size();

		int temp = vetor_fechadas[i];
		vetor_fechadas[i] = vetor_fechadas[r];
		vetor_fechadas[r] = temp;
}



}
else 
{   

fechadas_copia.clear();
abertas_copia.clear();
for (int j = 0; j < m; j++){
s_copia[j].clear();
if (! s[j].empty() )
{
abertas_copia.push_back(j);
for ( std::list<int>::iterator k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
// criar fo copia
fo_copia = fo;
iter = iter + 1;
k = k + 1;
   
}


} //fim ils

//cout << "solucao apos ils:" << fo << endl;

////////verifica se há alguma facilidade aberta que não está no conjunto de facilidades potenciais
list<int> lista_facilidades_potenciais;
for (int j = 0; j < facilidades_potenciais.size(); j++)
{
 lista_facilidades_potenciais.push_back(facilidades_potenciais[j]);
}

for ( std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++ ){
if ( !esta_na_lista(lista_facilidades_potenciais,*k))
{
    cout << "alterou facilidades potenciais!!" << endl;
    facilidades_potenciais.push_back(*k);
}
 
}



} 

void ILS_1_novo2(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>&facilidades_potenciais, vector<double> p, int tamanho_instancia, vector<vector<int>> matriz_clientes_ordenados, double menor_demanda , double soma_das_demandas, int ils_max1)
{
//cout << "COMEÇA ILS-I NOVO ____________________________________________________________________" << endl;
//cout << "abertas:" << endl;
//imprime_lista(abertas);
//cout << endl;
//cout << "fechadas:" << endl;
//imprime_lista(fechadas);
//cout << endl;
//cout << "valor do fo no inicio: " << fo << endl;

//int vezesmax = ILSmax; 
//list<int> locais_proibidos;
vector<list<int>> s_copia(m);
vector<int> atende_copia(n);
vector<double> pen_copia(m);
list<int> abertas_copia;
list<int> fechadas_copia;
double fo_copia;
// criar s_copia abertas_copia e fechadas_copia
for (int j = 0; j < m; j++){
if (! s[j].empty() )
{
abertas_copia.push_back(j);

for ( std::list<int>::iterator  k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
// criar fo copia e atende copia
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
fo_copia = fo;


int iter = 0;


vector<int> facilidades_nao_potenciais;
list<int> lista_facilidades_potencias;
for (int i = 0; i < facilidades_potenciais.size(); i++)
{
  lista_facilidades_potencias.push_back(facilidades_potenciais[i]);
}
for (int j = 0; j < m; j++)
{
    if ( !esta_na_lista(lista_facilidades_potencias, j))
    {
        facilidades_nao_potenciais.push_back(j);
    }
}



for (int i = 0; i < facilidades_nao_potenciais.size(); i++)
{
		int r = rand() % facilidades_nao_potenciais.size();

		int temp = facilidades_nao_potenciais[i];
		facilidades_nao_potenciais[i] = facilidades_nao_potenciais[r];
		facilidades_nao_potenciais[r] = temp;
}
int auxx = facilidades_nao_potenciais.size();
ils_max1 = min(ils_max1, auxx);
//cout << "ils max 1 = " << ils_max1 << endl;
int k = 0;
int s1;
vector<int> vetor_clientes(n);
for (int i = 0; i < n; i++)
{
    vetor_clientes[i] = i;
}
while(iter < ils_max1)
{

double fo1 = 0;
double fo2 = 0;






for ( std::list<int>::iterator k = abertas_copia.begin(); k != abertas_copia.end(); k++ ){

    fo1 = fo1 + f[*k];
}



//cout << "custos fixos abertura antes perturbacao:" << fo1 << endl;

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < m; j++)
    {
        if (atende_copia[i] == j)
        {
            fo2 = fo2 + c[i][j]*d[i];
        }
    }
    
}

s1 = facilidades_nao_potenciais[k];

//cout << "custos atendimento antes perturbacao:" << fo2 << endl;


//cout << "confere solucao s antes de fazer a perturbacao:" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;   

//sorteia duas facilidades fechadas
/*
int s1 = rand() % m;

while (esta_na_lista(abertas_copia,s1) || esta_na_lista(locais_proibidos,s1))
{
    s1 = rand() % m;
}
locais_proibidos.push_back(s1);
*/


/*
int s2 = rand() % m;
while (esta_na_lista(abertas_copia,s2) || (s2 == s1))
{
    s2 = rand() % m;
}
*/
/*
int s3 = rand() % m;
while (esta_na_lista(abertas_copia,s3) || (s3 == s1) || (s3 == s2) )
{
    s3 = rand() % m;
}
*/

//cout << "facilidades abertas: " << s1 <<  endl;
abertas_copia.push_back(s1);
//abertas_copia.push_back(s2);
//abertas_copia.push_back(s3);

//fechadas_copia.remove(s3);
fechadas_copia.remove(s1);
//fechadas_copia.remove(s2);
fo1 = fo1 + f[s1];
//fo1 = fo1 + f[s1] + f[s2]
//fo1 = fo1 + f[s1] + f[s2] + f[s3];
//fo_copia = fo_copia + f[s1] + f[s2];


//realoca para s1 e s2 clientes mais próximos até que a capacidade esgote
int i = 0;
int conta1 = 0;
int conta2 = 0;
while ( i < n )
{
    if ( (pen_copia[s1] + d[matriz_clientes_ordenados[s1][i]] <= 0) && (s_copia[atende_copia[matriz_clientes_ordenados[s1][i]]].size() > 1 )   )
    {
        
        s_copia[atende_copia[matriz_clientes_ordenados[s1][i]]].remove(matriz_clientes_ordenados[s1][i]);
        pen_copia[atende_copia[matriz_clientes_ordenados[s1][i]]] = pen_copia[atende_copia[matriz_clientes_ordenados[s1][i]]] - d[matriz_clientes_ordenados[s1][i]];
        pen_copia[s1] = pen_copia[s1] + d[matriz_clientes_ordenados[s1][i]];
        s_copia[s1].push_back(matriz_clientes_ordenados[s1][i]);
        fo2 = fo2 - c[matriz_clientes_ordenados[s1][i]][atende_copia[matriz_clientes_ordenados[s1][i]]]*d[matriz_clientes_ordenados[s1][i]] ;
        fo2 = fo2 + c[matriz_clientes_ordenados[s1][i]][s1]*d[matriz_clientes_ordenados[s1][i]] ;
        atende_copia[matriz_clientes_ordenados[s1][i]] = s1;
        conta1 = conta1 + 1;
    }
      i = i + 1;
      if ((abs(pen_copia[s1])) < menor_demanda)
      {
        i = n;
      }

}
/*
 i = 0;

while ( i < n )
{
    //cout << i << endl;
    if ( (pen_copia[s2] + d[matriz_clientes_ordenados[s2][i]] <= 0) &&  (s_copia[atende_copia[matriz_clientes_ordenados[s2][i]]].size() > 1 ) )
    {
        //realoca
        s_copia[atende_copia[matriz_clientes_ordenados[s2][i]]].remove(matriz_clientes_ordenados[s2][i]);
        pen_copia[s2] = pen_copia[s2] + d[matriz_clientes_ordenados[s2][i]];
        s_copia[s2].push_back(matriz_clientes_ordenados[s2][i]);
        fo2 = fo2 - c[matriz_clientes_ordenados[s2][i]][atende_copia[matriz_clientes_ordenados[s2][i]]]*d[matriz_clientes_ordenados[s2][i]];
        fo2 = fo2 + c[matriz_clientes_ordenados[s2][i]][s2]*d[matriz_clientes_ordenados[s2][i]];
        atende_copia[matriz_clientes_ordenados[s2][i]] = s2;
        conta2 = conta2 + 1;
    }
     i = i + 1;
         if (pen_copia[s2] == 0)
      {
        i = n;
      }
}
*/

 //i = 0;
/*
while ( i < n )
{
    //cout << i << endl;
    if ( (pen_copia[s3] + d[matriz_clientes_ordenados[s3][i]] <= 0) &&  (s_copia[atende_copia[matriz_clientes_ordenados[s3][i]]].size() > 1 ) )
    {
        //realoca
        s_copia[atende_copia[matriz_clientes_ordenados[s3][i]]].remove(matriz_clientes_ordenados[s3][i]);
        pen_copia[s3] = pen_copia[s3] + d[matriz_clientes_ordenados[s3][i]];
        s_copia[s3].push_back(matriz_clientes_ordenados[s3][i]);
        fo2 = fo2 - c[matriz_clientes_ordenados[s3][i]][atende_copia[matriz_clientes_ordenados[s3][i]]]*d[matriz_clientes_ordenados[s3][i]];
        fo2 = fo2 + c[matriz_clientes_ordenados[s3][i]][s3]*d[matriz_clientes_ordenados[s3][i]];
        atende_copia[matriz_clientes_ordenados[s3][i]] = s3;
        conta2 = conta2 + 1;
    }
     i = i + 1;
         if (pen_copia[s3] == 0)
      {
        i = n;
      }
}
*/

fo_copia = fo1 + fo2;
//cout << endl;
//cout << "fo apos perturbacao: " << fo_copia << endl;
//cout << "confere solucao s" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;
//cout << "penalidade primeira: " << pen_copia[s1] << endl;
//cout << "penalidade segunda: " << pen_copia[s2] << endl;




/*
 fo1 = 0;
 fo2 = 0;

for ( std::list<int>::iterator k = abertas_copia.begin(); k != abertas_copia.end(); k++ )
{
    fo1 = fo1 + f[*k];
}

cout << "custos fixos abertura depois perturbacao:" << fo1 << endl;

for (int i = 0; i < n; i++)
{
    for (int j = 0; j < m; j++)
    {
        if (atende_copia[i] == j)
        {
            fo2 = fo2 + c[i][j]*d[i];
        }
    }
    
}
*/


//cout << "custos atendimento depois  perturbacao:" << fo2 << endl;


//cout << "clientes realocados em : " << s1 << ":" << conta1 << endl;
//cout << "clientes realocados em : " << s2 << ":" << conta2 << endl;






//confere_solucao_atende (m, n, c,f,  atende_copia,d);
//cout << endl;


//cout << "_________________________________________________________________________________" << endl;

bool factivel = true;
//list<int> 
VND_factivel_1_2_3( m, n, s_copia , fo_copia, pen_copia, atende_copia, c,d,f, abertas_copia, fechadas_copia, vetor_clientes, factivel,  abertas_copia, p, menor_demanda) ;

//cout << "fo apos busca local: " << fo_copia << endl;
//cout << "confere solucao" << endl;
//confere_solucao_atende (m, n, c,f,  atende_copia,d);
//cout << endl;
//cout << "confere solucao s" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;
//exit(0);
//cout << "__________________________________________________________________________________" << endl;
if (fo_copia < 0.99999999999*fo) //houve melhora fica com a solucao
{


//locais_proibidos.clear();
abertas.clear();
fechadas.clear();
for (int j = 0; j < m; j++){
s[j].clear();
if (! s_copia[j].empty() )
{
abertas.push_back(j);
for ( std::list<int>::iterator k = s_copia[j].begin(); k != s_copia[j].end(); k++ ){
s[j].push_back(*k); 
}
}
else
{
fechadas.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen[j] = pen_copia[j];
} 
for (int i = 0; i < n; i++)
{
    atende[i] = atende_copia[i];
} 
fo = fo_copia;
iter = 0;
//cout << "fo melhor: " << fo << endl;
k = 0;




facilidades_nao_potenciais.clear();
lista_facilidades_potencias.clear();
for (int i = 0; i < facilidades_potenciais.size(); i++)
{
  lista_facilidades_potencias.push_back(facilidades_potenciais[i]);
}
for (int j = 0; j < m; j++)
{
    if ( !esta_na_lista(lista_facilidades_potencias, j))
    {
        facilidades_nao_potenciais.push_back(j);
    }
}



for (int i = 0; i < facilidades_nao_potenciais.size(); i++)
{
		int r = rand() % facilidades_nao_potenciais.size();

		int temp = facilidades_nao_potenciais[i];
		facilidades_nao_potenciais[i] = facilidades_nao_potenciais[r];
		facilidades_nao_potenciais[r] = temp;
}



}
else 
{   

fechadas_copia.clear();
abertas_copia.clear();
for (int j = 0; j < m; j++){
s_copia[j].clear();
if (! s[j].empty() )
{
abertas_copia.push_back(j);
for ( std::list<int>::iterator k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
// criar fo copia
fo_copia = fo;
iter = iter + 1;
k = k + 1;
   
}


} //fim ils

//cout << "solucao apos ils:" << fo << endl;

////////verifica se há alguma facilidade aberta que não está no conjunto de facilidades potenciais
list<int> lista_facilidades_potenciais;
for (int j = 0; j < facilidades_potenciais.size(); j++)
{
 lista_facilidades_potenciais.push_back(facilidades_potenciais[j]);
}

for ( std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++ ){
if ( !esta_na_lista(lista_facilidades_potenciais,*k))
{
    cout << "alterou facilidades potenciais!!" << endl;
    facilidades_potenciais.push_back(*k);
}
 
}



} 

void best_improvement_movimento_abre(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis, vector<int> facilidades_potenciais, vector<double> p, vector<vector<int>> &matriz_clientes_ordenados, double menor_demanda) 
{

bool pare2 = false;
vector<list<int>> s_antes(m);
vector<double> pen_antes(m);
vector<int> atende_antes(n);
list<int> abertas_antes;
list<int> fechadas_antes;
vector<list<int>> s_copia(m);
vector<int> atende_copia(n);
vector<double> pen_copia(m);
vector<int> atende_melhor(n);
list<int> abertas_copia;
list<int> fechadas_copia;
double fo_copia;
double fo_antes;
double fo_melhor = fo;
int melhor_abre;
bool houve_melhora = false;


while (pare2 == false)
{


houve_melhora = false;
abertas_antes.clear();
abertas_copia.clear();
fechadas_antes.clear();
fechadas_copia.clear();

// criar s_copia abertas_copia e fechadas_copia
for (int j = 0; j < m; j++){
s_copia[j].clear();
s_antes[j].clear();
if (! s[j].empty() )
{
abertas_antes.push_back(j);
abertas_copia.push_back(j);

for ( std::list<int>::iterator  k = s[j].begin(); k != s[j].end(); k++ ){
s_antes[j].push_back(*k); 
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_antes.push_back(j);
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_antes[j] = pen[j];
    pen_copia[j] = pen[j];

} 

for (int i = 0; i < n; i++)
{
    atende_antes[i] = atende[i];
    atende_copia[i] = atende[i];
} 
fo_antes = fo;
fo_copia = fo;

vector<int> facilidades_potenciais_fechadas;

for (int i = 0; i < facilidades_potenciais.size(); i++)
{

 //std::list<int>::iterator k = find(abertas.begin(), abertas.end(), facilidades_potenciais[i]);
  if (  !esta_na_lista(abertas,facilidades_potenciais[i])   )
 //if (*k != facilidades_potenciais[i])
 {
     
     facilidades_potenciais_fechadas.push_back(facilidades_potenciais[i]);
 }
}

//percorre toda a vizinhanca e encontra o melhor vizinho
/*
cout << "--------------------------------EXPLORCAO DE VIZINHANCA------------------------------------------" << endl;
for (int j = 0; j < facilidades_potenciais_fechadas.size(); j++)
{
   cout << facilidades_potenciais_fechadas[j] << " ";
}
cout << endl;
*/

for (int j = 0; j < facilidades_potenciais_fechadas.size(); j++) 
{


//cout << "tenta abrir" << facilidades_potenciais_fechadas[j] << endl;
//cout << " fo copia antes" << fo_copia << endl;
double limitante = 0;
fo_copia = fo_copia + f[facilidades_potenciais_fechadas[j]];
abertas_copia.push_back(facilidades_potenciais_fechadas[j]);
fechadas_copia.remove(facilidades_potenciais_fechadas[j]);
bool pare = false;
//cout << "novo fo" << fo_copia << endl;

int conta = 0;
for (int i = 0; i < n; i++)
{
    int cliente_de_menor_custo = matriz_clientes_ordenados[facilidades_potenciais_fechadas[j]][i];
if ( pen_copia[facilidades_potenciais_fechadas[j]] + d[cliente_de_menor_custo]  <= 0 )
{
   conta = conta + 1;
   //cout << "aloca cliente " << cliente_de_menor_custo << endl; 
   if (s_copia[atende_copia[cliente_de_menor_custo]].size() < 1.1)
   {
      // cout << "causou fechamento!!!!!!!!!!!!!!!!!!!!!!!!!!!! de: " << atende_copia[cliente_de_menor_custo] << endl;
       fo_copia = fo_copia - f[atende_copia[cliente_de_menor_custo]];
       fechadas_copia.push_back(atende_copia[cliente_de_menor_custo]);
       abertas_copia.remove(atende_copia[cliente_de_menor_custo]);

   }
   s_copia[atende_copia[cliente_de_menor_custo]].remove(cliente_de_menor_custo);
   s_copia[facilidades_potenciais_fechadas[j]].push_back(cliente_de_menor_custo);
   pen_copia[atende_copia[cliente_de_menor_custo]] = pen_copia[atende_copia[cliente_de_menor_custo]] - d[cliente_de_menor_custo];
   pen_copia[facilidades_potenciais_fechadas[j]] = pen_copia[facilidades_potenciais_fechadas[j]] + d[cliente_de_menor_custo];
   fo_copia = fo_copia - c[cliente_de_menor_custo][atende_copia[cliente_de_menor_custo]]*d[cliente_de_menor_custo] + c[cliente_de_menor_custo][facilidades_potenciais_fechadas[j]]*d[cliente_de_menor_custo];
   atende_copia[cliente_de_menor_custo] = facilidades_potenciais_fechadas[j];
   //cout << "novo fo" << fo_copia << endl;
  



}
else
{
   //cout << "cliente não coube " << endl; 
   break;
}

}




if ( (conta > 0) && (fo_copia < 1.05*fo))
{
//cout << "fo intermediario " << fo_copia << endl;
//double confere = confere_solucao_atende (m,n, c,f,atende_copia, d);
//cout << "confere: " << confere << endl;

VND_factivel_1_2_3(m, n,s_copia ,fo_copia,pen_copia,atende_copia,c, d,f, abertas_copia,fechadas_copia, vetor_clientes ,factivel, abertas_copia, p, menor_demanda); 

cout << "fo apos busca local " << fo_copia << endl;
confere_solucao_atende(m,n, c,f,atende_copia, d);

if (fo_copia < 0.9999*fo_melhor) //houve melhora fica com a solucao
{
fo_melhor = fo_copia;
melhor_abre = facilidades_potenciais_fechadas[j]; 
houve_melhora = true;
for (int i = 0; i < n; i++)
{
    atende_melhor[i] = atende_copia[i];
}


}
}
//volta tudo o que era antes

abertas_copia.clear();
fechadas_copia.clear();
for (int j = 0; j < m; j++){
s_copia[j].clear();

if (!s_antes[j].empty() )
{
abertas_copia.push_back(j);

for ( std::list<int>::iterator  k = s_antes[j].begin(); k != s_antes[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    
    pen_copia[j] = pen_antes[j];

} 

for (int i = 0; i < n; i++)
{
        atende_copia[i] = atende_antes[i];
} 
fo_copia = fo_antes;





} //fim exploracao de vizinhanca
if (houve_melhora == true)  //faz o movimento
{
cout << " Faz o movimento:" << fo_melhor << endl;
cout << "Abre: " << melhor_abre << endl;


fo = fo_melhor;
    abertas.clear();
    fechadas.clear();
for (int j = 0; j < m; j++)
{
    pen[j] = -p[j];
    s[j].clear();

    for (int i = 0; i < n; i++)
    {
        if (atende_melhor[i] == j)
    {
        atende[i] = j;   
        s[j].push_back(i);
        pen[j] = pen[j] + d[i];
    }
    }
    if (!s[j].empty())
    {
     abertas.push_back(j);
    }
    else
    {
     fechadas.push_back(j);
    }

}

}
else
{
pare2 = true;
}




} //fim while pare é false

} //fim void explora vizinhanca

void clientes_ordenados(int m, int n, vector<vector<double>> c, vector<vector<int>> &matriz_clientes_ordenados)
{
//para cada faclilidades ordena os clientes na ordem crescente de proximidade
//matriz clientes ordenados inicial é feita na leitura de instancias
for (int j = 0; j < m; j++)
{

for (int i1 = 0; i1 < n - 1; i1++)
{
    //cout << "posicao " << i1 << endl;
    int menor_custo = i1;
    for (int i2 = i1+1; i2 < n; i2++)
    {
        if (c[matriz_clientes_ordenados[j][i2]][j] < c[matriz_clientes_ordenados[j][menor_custo]][j] )
        {
            menor_custo = i2; //indice de menor custo
        }
    }
    //cout << "Cliente de menor custo: " << menor_custo << "Custo:" << c[matriz_clientes_ordenados[j][menor_custo]][j] << endl;
     if (menor_custo != i1) //coloca menor custo na posicao i1
    {
            int temp = matriz_clientes_ordenados[j][i1]; //cliente que ocupa posicao i1
            matriz_clientes_ordenados[j][i1] = matriz_clientes_ordenados[j][menor_custo];
            matriz_clientes_ordenados[j][menor_custo] = temp;
    }   

}


}

}

void facilidades_ordenadas(int m, int n, vector<vector<double>> c, vector<vector<int>> &matriz_facilidades_ordenadas)
{
//para cada faclilidades ordena os clientes na ordem crescente de proximidade
//matriz clientes ordenados inicial é feita na leitura de instancias

for (int i = 0; i < n; i++)
{

for (int j1 = 0; j1 < m - 1; j1++)
{
    //cout << "posicao " << i1 << endl;
    int menor_custo = j1;
    for (int j2 = j1+1; j2 < m; j2++)
    {
        if (c[i][matriz_facilidades_ordenadas[i][j2]] < c[i][matriz_facilidades_ordenadas[i][menor_custo]] )
        {
            menor_custo = j2; //indice de menor custo
        }
    }
    //cout << "Cliente de menor custo: " << menor_custo << "Custo:" << c[matriz_clientes_ordenados[j][menor_custo]][j] << endl;
     if (menor_custo != j1) //coloca menor custo na posicao i1
    {
            int temp = matriz_facilidades_ordenadas[i][j1]; //cliente que ocupa posicao i1
            matriz_facilidades_ordenadas[i][j1] = matriz_facilidades_ordenadas[i][menor_custo];
            matriz_facilidades_ordenadas[i][menor_custo] = temp;
    }   

}


}



}

void ordena_clientes_decrescente_demanda(vector<int>&vetor_clientes, vector<double> d)
{
 for (int i = 0; i < vetor_clientes.size()- 1; i++)
    {
    int maior_demanda = i;
    for (int j = i; j < vetor_clientes.size(); j++)
    {
       if (d[vetor_clientes[j]] > d[vetor_clientes[maior_demanda]])
       {
           maior_demanda = j;
       }
    }
    if (maior_demanda != i)
    {
        int temp = vetor_clientes[i];
        vetor_clientes[i] = vetor_clientes[maior_demanda];
        vetor_clientes[maior_demanda] = temp;
    }
    
    }

}

void aloca_clientes(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, double menor_demanda, double soma_das_demandas, vector<int> melhor_atende_relaxacao )
{
//aloca conjunto de clientes em facilidades abertas disponíveis
double fo1 = 0;
double fo2 = 0;

//cout << "soma custos de: ";
for (std::list<int>::iterator k = abertas_disponiveis.begin(); k != abertas_disponiveis.end(); k++ )
{
    fo1 = fo1 + f[*k];

    //cout << *k << " ";
}
//cout << endl;
conseguiu_alocar = true;
for (int j = 0; j < m; j++)
{
    pen[j] = -p[j];
}


bool alocou_cliente;
int conta = 0;
for (int i = 0; i < vetor_clientes.size(); i++)
{
   alocou_cliente = false;
   for (int j = 0; j < m; j++)
   {

    if ( (pen[melhor_atende_relaxacao[vetor_clientes[i]]] + d[vetor_clientes[i]] <= 0) && (esta_na_lista(abertas_disponiveis, melhor_atende_relaxacao[vetor_clientes[i]]      ))   )
    {
        atende[vetor_clientes[i]] = melhor_atende_relaxacao[vetor_clientes[i]];
        fo2 = fo2 + c[vetor_clientes[i]][atende[vetor_clientes[i]]]*d[vetor_clientes[i]];
        pen[melhor_atende_relaxacao[vetor_clientes[i]]] = pen[melhor_atende_relaxacao[vetor_clientes[i]]] + d[vetor_clientes[i]];
        alocou_cliente = true;
        conta = conta + 1;
        break;
    }
    if ( (pen[matriz_facilidades_ordenadas[vetor_clientes[i]][j]] + d[vetor_clientes[i]] <= 0) && (esta_na_lista(abertas_disponiveis, matriz_facilidades_ordenadas[vetor_clientes[i]][j]    ))   )
    {
        atende[vetor_clientes[i]] = matriz_facilidades_ordenadas[vetor_clientes[i]][j];
        fo2 = fo2 + c[vetor_clientes[i]][atende[vetor_clientes[i]]]*d[vetor_clientes[i]];
        pen[matriz_facilidades_ordenadas[vetor_clientes[i]][j]] = pen[matriz_facilidades_ordenadas[vetor_clientes[i]][j]] + d[vetor_clientes[i]];
        alocou_cliente = true;
        conta = conta + 1;
        break;
    }
   }
   if (alocou_cliente == false)
   {
     conseguiu_alocar = false;
     break;
   }
       
    
}
//cout << "custos abertura:" << fo1 << endl;
//cout << "custos atendimento:" << fo2 << endl;
fo = fo1 + fo2;
//cout << conta << endl;

}

void movimentos_fecha_abre_apos_solucao_inicial(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> &abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, list<int> &fechadas_disponiveis, double menor_demanda, double soma_das_demandas , vector<int> melhor_atende_relaxacao)
{



double capacidade_total_disponivel = 0;    
for (std::list<int>::iterator k = abertas_disponiveis.begin(); k != abertas_disponiveis.end(); k++)
{
capacidade_total_disponivel = capacidade_total_disponivel + p[*k];
} 
cout << "capacidade total redisual: " << capacidade_total_disponivel - soma_das_demandas << endl;



 //cout << "abertas_disponiveis" << endl; 
 //imprime_lista(abertas_disponiveis);
 //cout << endl; 
 abertas_disponiveis.reverse();
 // imprime_lista(abertas_disponiveis);
 //cout << endl; 
 list<int> abertas_disponiveis_copia;


 list<int> fechadas_disponiveis_copia;

 
 list<int> abertas_auxiliar;
 cria_copia_lista(abertas_auxiliar, abertas_disponiveis);
 
 list<int> fechadas_auxiliar;
  cria_copia_lista(fechadas_auxiliar, fechadas_disponiveis);
 
 double fo_melhor = fo;
 vector<int> atende_melhor(n);
 cria_copia_vetor_int(atende_melhor,atende);
 list<int> abertas_melhor;
 list<int> fechadas_melhor;


 cria_copia_lista(abertas_melhor, abertas_disponiveis);
 

  cria_copia_lista(fechadas_melhor, fechadas_disponiveis);
 bool pare = false;
 /*
cout << "abertas:" << endl;
imprime_lista(abertas_disponiveis);
cout << endl;
cout << "fechadas:" << endl;
imprime_lista(fechadas_disponiveis);
cout << endl;
*/
bool houve_melhora = true;
while (pare == false)
{

cout << "abertas_melhor:" << endl;
imprime_lista(abertas_melhor);
cout << endl;
cout << "fechadas_melhor:" << endl;
imprime_lista(fechadas_melhor);
cout << endl;


cria_copia_lista(abertas_disponiveis_copia, abertas_melhor);
cria_copia_lista(fechadas_disponiveis_copia, fechadas_melhor);


if (houve_melhora == false) 
{
pare = true;
continue;
}
cout << "Começa exploração de Vizinhança" << endl;


houve_melhora = false;

bool sai_do_loop = false; //recomeça a exporacao da vizinhanca
 for ( std::list<int>::iterator k1 = abertas_disponiveis_copia.begin(); k1 != abertas_disponiveis_copia.end(); k1++ ){
        
        if (sai_do_loop == true)
        {
          
            abertas_auxiliar.push_back(*k1);
            fechadas_auxiliar.remove(*k1);
            capacidade_total_disponivel = capacidade_total_disponivel + p[*k1];
            break;
        }
            abertas_auxiliar.remove(*k1);
            fechadas_auxiliar.push_back(*k1);
        cout << "fecha: " << *k1 << " ";
        capacidade_total_disponivel = capacidade_total_disponivel - p[*k1];

        for ( std::list<int>::iterator k2 = fechadas_disponiveis_copia.begin(); k2 != fechadas_disponiveis_copia.end(); k2++ )
            {   
              
           
              cout << "abre: " << *k2 << endl;
              fechadas_auxiliar.remove(*k2);
              abertas_auxiliar.push_back(*k2);
             capacidade_total_disponivel = capacidade_total_disponivel + p[*k2];
              if (capacidade_total_disponivel < soma_das_demandas)
              {
              abertas_auxiliar.remove(*k2);
              fechadas_auxiliar.push_back(*k2);
              capacidade_total_disponivel = capacidade_total_disponivel - p[*k2];
              cout << "impossível alocar" << endl;
              //abertas_auxiliar.remove(*k1);
              //fechadas_auxiliar.push_back(*k1);
              //capacidade_total_disponivel = capacidade_total_disponivel + p[*k1];
              //break;//sai do for
              continue;
              }
              
 
              conseguiu_alocar = false;
               aloca_clientes(fo,  m, n,  c, matriz_facilidades_ordenadas, vetor_clientes, p,d, conseguiu_alocar, abertas_auxiliar, pen, atende, f, menor_demanda,  soma_das_demandas, melhor_atende_relaxacao);
            if (conseguiu_alocar == 1)
            {
            cout << "conseguiu alocar: " << conseguiu_alocar << " " << "fo: " << fo << endl;  
            }
            if ( (conseguiu_alocar == true) && (fo < fo_melhor) )
            {
                cout << "houve_melhora: " << fo << endl;
                
                fo_melhor = fo;
                houve_melhora = true;
                cria_copia_vetor_int(atende_melhor, atende);
                confere_solucao_atende (m,n,c, f, atende_melhor, d); 
                cout <<  endl;
                sai_do_loop = true;
                cria_copia_lista(abertas_melhor, abertas_auxiliar);
                cria_copia_lista(fechadas_melhor, fechadas_auxiliar);
                 break; //sai do for
                

            
            }
            else
            {
              //abertas_auxiliar.push_back(*k1);
              abertas_auxiliar.remove(*k2);
              //fechadas_auxiliar.remove(*k1);
              fechadas_auxiliar.push_back(*k2);
              capacidade_total_disponivel = capacidade_total_disponivel + p[*k2];

            }

            }
    }
 



}

cria_copia_vetor_int(atende, atende_melhor);
confere_solucao_atende (m,n,c, f, atende, d); 
cout <<  endl;
cria_copia_lista(abertas_disponiveis, abertas_melhor);
cria_copia_lista(fechadas_disponiveis, fechadas_melhor);
fo = fo_melhor;


}

void movimentos_abre_apos_solucao_inicial(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> &abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, list<int> &fechadas_disponiveis, double menor_demanda, double soma_das_demandas, vector<int> &novas_facilidades, vector<int> facilidades_potenciais, vector<int> melhor_atende_relaxacao)
{

 //cout << "abertas_disponiveis" << endl; 
 //imprime_lista(abertas_disponiveis);
 //cout << endl; 
 //abertas_disponiveis.reverse();
 // imprime_lista(abertas_disponiveis);
 //cout << endl; 
 list<int> abertas_disponiveis_copia;


 list<int> fechadas_disponiveis_copia;

 
 list<int> abertas_auxiliar;
 cria_copia_lista(abertas_auxiliar, abertas_disponiveis);
 
 list<int> fechadas_auxiliar;
  cria_copia_lista(fechadas_auxiliar, fechadas_disponiveis);
 
 double fo_melhor = fo;
 vector<int> atende_melhor(n);
 cria_copia_vetor_int(atende_melhor,atende);
 list<int> abertas_melhor;
 list<int> fechadas_melhor;


cria_copia_lista(abertas_melhor, abertas_disponiveis);
cria_copia_lista(fechadas_melhor, fechadas_disponiveis);
 bool pare = false;
 /*
cout << "abertas:" << endl;
imprime_lista(abertas_disponiveis);
cout << endl;
cout << "fechadas:" << endl;
imprime_lista(fechadas_disponiveis);
cout << endl;
*/
int conta_facilidades = 0;
bool houve_melhora = true;
while (pare == false)
{

cout << "abertas_melhor:" << endl;
imprime_lista(abertas_melhor);
cout << endl;
cout << "fechadas_melhor:" << endl;
imprime_lista(fechadas_melhor);
cout << endl;


cria_copia_lista(abertas_disponiveis_copia, abertas_melhor);
cria_copia_lista(fechadas_disponiveis_copia, fechadas_melhor);


if (houve_melhora == false) 
{
pare = true;
continue;
}
cout << "Começa exploração de Vizinhança Abre" << endl;

houve_melhora = false;

bool sai_do_loop = false; //recomeça a exporacao da vizinhanca

        for ( std::list<int>::iterator k2 = fechadas_disponiveis_copia.begin(); k2 != fechadas_disponiveis_copia.end(); k2++ )
            {
              
              cout << "abre: " << *k2 << endl;
                    fechadas_auxiliar.remove(*k2);
                    abertas_auxiliar.push_back(*k2);
            
              conseguiu_alocar = false;
               aloca_clientes(fo,  m, n,  c, matriz_facilidades_ordenadas, vetor_clientes, p,d, conseguiu_alocar, abertas_auxiliar, pen, atende, f, menor_demanda, soma_das_demandas, melhor_atende_relaxacao);
            if (conseguiu_alocar == 1)
            {
            cout << "conseguiu alocar: " << conseguiu_alocar << " " << "fo: " << fo <<  " fo melhor " << fo_melhor << endl; 
            }
            if ( (conseguiu_alocar == true) && (fo < fo_melhor) )
            {
                cout << "houve_melhora: " << fo << endl;
                
                fo_melhor = fo;
                houve_melhora = true;
                cria_copia_vetor_int(atende_melhor, atende);
                confere_solucao_atende (m,n,c, f, atende_melhor, d); 
                cout <<  endl;
                sai_do_loop = true;
                cria_copia_lista(abertas_melhor, abertas_auxiliar);
                cria_copia_lista(fechadas_melhor, fechadas_auxiliar);
                 break; //sai do for
                

            
            }
            else
            {
              
              abertas_auxiliar.remove(*k2);
              fechadas_auxiliar.push_back(*k2);

            }

            }
    
 



}

cria_copia_vetor_int(atende, atende_melhor);
cria_copia_lista(abertas_disponiveis, abertas_melhor);
cria_copia_lista(fechadas_disponiveis, fechadas_melhor);
confere_solucao_atende (m,n,c, f, atende, d); 
fo = fo_melhor;
cout <<  endl;

}

void movimentos_abre_apos_solucao_inicial_best_improvement(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> &abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, list<int> &fechadas_disponiveis, double menor_demanda, double soma_das_demandas, vector<int> &novas_facilidades, vector<int> &facilidades_potenciais, int quantidade_abertas_movimento1, vector<int> melhor_atende_relaxacao)
{

 //cout << "abertas_disponiveis" << endl; 
 //imprime_lista(abertas_disponiveis);
 //cout << endl; 
 //abertas_disponiveis.reverse();
 // imprime_lista(abertas_disponiveis);
 //cout << endl; 
 list<int> abertas_disponiveis_copia;

 list<int> lista_facilidades_potenciais;
 for ( int i = 0; i < facilidades_potenciais.size(); i++)
 {
    lista_facilidades_potenciais.push_back(facilidades_potenciais[i]);
 }
 //cout << "lista facilidades potenciais:" << endl;
 //imprime_lista(lista_facilidades_potenciais);
 //cout << endl;

 list<int> fechadas_disponiveis_copia;
 list<int> abertas_auxiliar;
 cria_copia_lista(abertas_auxiliar, abertas_disponiveis);
 list<int> fechadas_auxiliar;
  cria_copia_lista(fechadas_auxiliar, fechadas_disponiveis);
 
 double fo_melhor = fo;
 vector<int> atende_melhor(n);
 cria_copia_vetor_int(atende_melhor,atende);
 list<int> abertas_melhor;
 list<int> fechadas_melhor;
int melhor_abertura = 0;

//cria_copia_lista(abertas_melhor, abertas_disponiveis);
//cria_copia_lista(fechadas_melhor, fechadas_disponiveis);
 bool pare = false;
 /*
cout << "abertas:" << endl;
imprime_lista(abertas_disponiveis);
cout << endl;
cout << "fechadas:" << endl;
imprime_lista(fechadas_disponiveis);
cout << endl;
*/
 vector<int> atende_antes(n);
  vector<double> pen_antes(m);
vector<double> valores_objetivos;
int conta_facilidades = 0;
double melhor_valor_do_pool = 0;
double pior_valor_do_pool = 0;
bool houve_melhora = true;
bool primeira_exploracao_vizinhanca = true;
while (pare == false)
{

//cout << "abertas_melhor:" << endl;
//imprime_lista(abertas_melhor);
//cout << endl;
//cout << "fechadas_melhor:" << endl;
//imprime_lista(fechadas_melhor);
//cout << endl;


//cria_copia_lista(abertas_disponiveis_copia, abertas_melhor);
//cria_copia_lista(fechadas_disponiveis_copia, fechadas_melhor);

houve_melhora = false;


double fo_antes;
fo_antes = fo;

cria_copia_lista(fechadas_auxiliar, fechadas_disponiveis);
//cria_copia_lista(fechadas_auxiliar, fechadas_disponiveis);
cria_copia_vetor_int(atende_antes,atende);
for (int j = 0; j < m; j++)
{
pen_antes[j] = pen[j]; 
}



        for ( std::list<int>::iterator k2 = fechadas_disponiveis.begin(); k2 != fechadas_disponiveis.end(); k2++ )
            {
              
              //cout << "abre: " << *k2 << endl;
               
                    fechadas_auxiliar.remove(*k2);
                    abertas_auxiliar.push_back(*k2);
                    for (int j = 0; j < m; j++)
                        {
                        pen_antes[j] = pen[j]; 
                    }
                    cria_copia_vetor_int(atende_antes,atende);



            
                //if (conta_facilidades >= 5)
               // {
                    //ordena vetor facilidades
                    
                //exit(0);


                
                //cout << "teste1" << endl;



              conseguiu_alocar = false;
               aloca_clientes(fo,  m, n,  c, matriz_facilidades_ordenadas, vetor_clientes, p,d, conseguiu_alocar, abertas_auxiliar, pen_antes, atende_antes, f, menor_demanda, soma_das_demandas, melhor_atende_relaxacao);
                              //cout << "teste2" << endl; 

                /*
              if (  (!esta_na_lista(lista_facilidades_potenciais, *k2)) && (primeira_exploracao_vizinhanca == true))
              {
              novas_facilidades.push_back(*k2) ; 
              valores_objetivos.push_back(fo);
              conta_facilidades = conta_facilidades + 1;
              
                        for (int i = 0; i < novas_facilidades.size()- 1; i++)
                        {
                for (int j = i+1; j < novas_facilidades.size(); j++)
                    {
                        if (valores_objetivos[j] < valores_objetivos[i] )
                            {
                            int temp =  novas_facilidades[i];
                            novas_facilidades[i] = novas_facilidades[j];
                            novas_facilidades[j] = temp;
                            }
        
                    }
    
                }

                                   for (int i = 0; i <valores_objetivos.size()- 1; i++)
                        {
                for (int j = i+1; j < valores_objetivos.size(); j++)
                    {
                        if (valores_objetivos[i] > valores_objetivos[j] )
                            {
                            int temp =  valores_objetivos[i];
                            valores_objetivos[i] = valores_objetivos[j];
                            valores_objetivos[j] = temp;
                            }
        
                    }
    
                }
              
              }  
               */
            


               // cout << "conseguiu alocar: " << conseguiu_alocar << " " << "fo: " << fo <<  " fo melhor " << fo_melhor << endl;  
            if ( (conseguiu_alocar == true) && (fo < fo_melhor) )
            {
                //cout << "houve_melhora: " << fo << endl;
                
                fo_melhor = fo;
                melhor_abertura = *k2;
                houve_melhora = true;
                //cria_copia_vetor_int(atende_melhor, atende);
                //confere_solucao_atende (m,n,c, f, atende_melhor, d); 
                //cout <<  endl;
                //sai_do_loop = true;
                //cria_copia_lista(abertas_melhor, abertas_auxiliar);
                //cria_copia_lista(fechadas_melhor, fechadas_auxiliar);
                 //sai do for
       

            
            }

              abertas_auxiliar.remove(*k2);
              fechadas_auxiliar.push_back(*k2);
              fo = fo_antes;

} //fim exploracao vizinhanca
primeira_exploracao_vizinhanca = false;
if (houve_melhora == true)
{
//cout << "houve melhora" << endl;
abertas_disponiveis.push_back(melhor_abertura);
fechadas_disponiveis.remove(melhor_abertura);

aloca_clientes(fo,  m, n,  c, matriz_facilidades_ordenadas, vetor_clientes, p,d, conseguiu_alocar, abertas_disponiveis, pen, atende, f, menor_demanda, soma_das_demandas, melhor_atende_relaxacao);

//confere_solucao_atende (m,n,c, f, atende, d); 

//cria_copia_vetor_int(atende, atende_melhor);
//cria_copia_lista(abertas_disponiveis, abertas_melhor);
//cria_copia_lista(fechadas_disponiveis, fechadas_melhor);
//confere_solucao_atende (m,n,c, f, atende, d); 
//fo = fo_melhor;
//cout <<  endl;
}
else
{
    pare = true;
}




}
//cout << endl;
//cout << "confere antes de sair do loop" << endl;
//confere_solucao_atende (m,n,c, f, atende, d); 
//cout << endl;
vector<list<int>> s(m);
for (int i = 0; i < n; i++)
{
   s[atende[i]].push_back(i);
}
//confere_solucao_s( m, n,  c,f,s,d,abertas_disponiveis, fechadas_disponiveis,  pen,  p );
//cout << endl;

}

void ILS_2 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>facilidades_potenciais, int tam_facilidades_potenciais, vector<double> p, int tamanho_instancia, int ils_max2, int constante_trocas)
{

double fstar = fo;
int iter = 0;
int melhor_iter = iter;
int nivel = 1;
int nvezes = 1; 
//int ILSmax = 120;
int vezesmax = 1; 
vector<list<int>> s_copia(m);
vector<int> atende_copia(n);
vector<double> pen_copia(m);
list<int> abertas_copia;
list<int> fechadas_copia;
double fo_copia;

for (int j = 0; j < tam_facilidades_potenciais; j++){
if (! s[facilidades_potenciais[j]].empty() )
{
abertas_copia.push_back(facilidades_potenciais[j]);
 
for ( std::list<int>::iterator k= s[facilidades_potenciais[j]].begin(); k != s[facilidades_potenciais[j]].end(); k++ ){
s_copia[facilidades_potenciais[j]].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(facilidades_potenciais[j]);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
fo_copia = fo;


while (iter - melhor_iter < ils_max2 )
{

iter = iter + 1;

int numero_abertas = 0;
for (int j = 0; j < m; j++)
{
    if (!s[j].empty())
    {
        numero_abertas = numero_abertas +1;
    }
}


vector<int> facilidades(abertas.size());
int i = 0; 
for (std::list<int>::iterator k = abertas.begin(); k != abertas.end(); k++ )
  {
      facilidades[i] = *k;
      i = i+1;
  }

//list<int> lista_clientes_perturbados;
int trocas = 0;
int cliente_sorteado1;
int cliente_sorteado2;
//int numero_trocas = n*gamma
while (trocas < constante_trocas*nivel)   
{
    cliente_sorteado1 = rand() % n;  //sorteia um clientes
    cliente_sorteado2 = (rand()+1) % n; // sorteia um outro cliente
 
    if ( (pen_copia[atende_copia[cliente_sorteado1]] + d[cliente_sorteado2] - d[cliente_sorteado1] <= 0) && (pen_copia[atende_copia[cliente_sorteado2]] + d[cliente_sorteado1] - d[cliente_sorteado2] <= 0) && (cliente_sorteado1 != cliente_sorteado2) &&  (atende_copia[cliente_sorteado1] != atende_copia[cliente_sorteado2])  )
    {
    fo_copia = fo_copia - d[cliente_sorteado1]*c[cliente_sorteado1][atende_copia[cliente_sorteado1]] + d[cliente_sorteado1]*c[cliente_sorteado1][atende_copia[cliente_sorteado2]]; 
    fo_copia = fo_copia - d[cliente_sorteado2]*c[cliente_sorteado2][atende_copia[cliente_sorteado2]] + d[cliente_sorteado2]*c[cliente_sorteado2][atende_copia[cliente_sorteado1]]; 

    //cout << "fo apos o movmento" << fo_copia << endl ;
    pen_copia[atende_copia[cliente_sorteado1]] = pen_copia[atende_copia[cliente_sorteado1]] - d[cliente_sorteado1] + d[cliente_sorteado2];
    pen_copia[atende_copia[cliente_sorteado2]] = pen_copia[atende_copia[cliente_sorteado2]] - d[cliente_sorteado2] + d[cliente_sorteado1] ;
   
    s_copia[atende_copia[cliente_sorteado1]].remove(cliente_sorteado1);
    s_copia[atende_copia[cliente_sorteado2]].remove(cliente_sorteado2);
    s_copia[atende_copia[cliente_sorteado1]].push_back(cliente_sorteado2);
    s_copia[atende_copia[cliente_sorteado2]].push_back(cliente_sorteado1);

  
    int temp = atende_copia[cliente_sorteado1]; 
    atende_copia[cliente_sorteado1] = atende_copia[cliente_sorteado2];
    atende_copia[cliente_sorteado2] = temp;
    trocas = trocas + 1;
    }
    
    else
    {
        trocas = trocas + 1;
    }


}
//cout << "fo apos a perturbaco" << fo_copia << endl ;
//cout << "confere solucao s" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;
//lista_clientes_perturbados.sort();
//lista_clientes_perturbados.unique();
vector<int> vetor_clientes;
for (int i = 0; i < n; i++)
{
    vetor_clientes.push_back(i);
}
bool factivel = true;
list<int> abertas_disponiveis;
for ( std::list<int>::iterator k = abertas_copia.begin(); k != abertas_copia.end(); k++)
{
    abertas_disponiveis.push_back(*k);
}

VND_factivel_1_2_3_ils( m, n,s_copia ,fo_copia,pen_copia, atende_copia, c, d, f,abertas_copia, fechadas_copia, vetor_clientes ,factivel,  abertas_disponiveis, p); 

//cout << "fo apos o vnd" << fo_copia << endl ;
//cout << "confere solucao s" << endl;
//confere_solucao_s( m, n, c,  f, s_copia, d, abertas_copia, fechadas_copia,pen_copia,  p );
//cout << endl;
//VND_ils2(m, n, s_copia , fo_copia, pen_copia,atende_copia, c,  d,  f, abertas_copia, fechadas_copia, p);

if (fo_copia < 0.9999999*fstar) //houve melhora fica com a solucao
{
    
melhor_iter = iter;
nivel = 1;
nvezes = 1; 
fstar = fo_copia;


abertas.clear();
fechadas.clear();
for (int j = 0; j < m; j++){
s[j].clear();
if (! s_copia[j].empty() )
{
abertas.push_back(j);
for ( std::list<int>::iterator k = s_copia[j].begin(); k != s_copia[j].end(); k++ ){
s[j].push_back(*k); 
}
}
else
{
fechadas.push_back(j);
}
}
// criar pen copia
for (int j = 0; j < m; j++)
{
    pen[j] = pen_copia[j];
} 
for (int i = 0; i < n; i++)
{
    atende[i] = atende_copia[i];
} 
fo = fstar; 
}

else 
{   //se nao houve melhora

fechadas_copia.clear();
abertas_copia.clear();
for (int j = 0; j < m; j++){
s_copia[j].clear();
if (! s[j].empty() )
{
abertas_copia.push_back(j);
for ( std::list<int>::iterator k = s[j].begin(); k != s[j].end(); k++ ){
s_copia[j].push_back(*k); 
}
}
else
{
fechadas_copia.push_back(j);
}
}

for (int j = 0; j < m; j++)
{
    pen_copia[j] = pen[j];
} 
for (int i = 0; i < n; i++)
{
    atende_copia[i] = atende[i];
} 
// criar fo copia
fo_copia = fo;

nivel = nivel + 1;
/*
if (nvezes >= vezesmax){ 
        nivel = nivel + 1;  
        nvezes = 1;        
    }
else
    {
    nvezes = nvezes + 1;
    }
    */
    
}
} 

//cout << "fim ils2" << endl;

}

void imprime_vetor(vector<int> vetor)
{
    for (int i = 0; i < vetor.size(); i++)
    {
       cout << vetor[i] << " ";
    }
    cout << endl;
}

void imprime_vetor_double(vector<double> vetor)
{
    for (int i = 0; i < vetor.size(); i++)
    {
       cout << vetor[i] << " ";
    }
    cout << endl;
}

void imprime_lista(list<int> lista)
{
for (  std::list<int>::iterator k = lista.begin() ; k !=  lista.end() ; k++)
{
cout << *k << " ";
}

}
