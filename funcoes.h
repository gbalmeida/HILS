// arquivo funcoes.h

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
#include <ilcplex/ilocplex.h>

using namespace std;



bool esta_na_lista(const std::list<int>& lista, int numero);

void cria_copia_lista (list<int> &lista_nova, list<int> lista);

void cria_copia_vetor_int(vector<int> &vetor_novo, vector<int> vetor);

void aloca_clientes(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> abertas_disponiveis, vector<double> &pen, vector<int> &atende, vector<double> f, double menor_demanda, double soma_das_demandas, vector<int> melhor_atende_relaxacao);

void confere_solucao_s(int m, int n, vector<vector<double>> c,  vector<double> f, vector<list<int>>s, vector<double> d, list<int> abertas, list<int> fechadas, vector<double> pen, vector<double> p );

void confere_solucao_atende (int m, int n, vector<vector<double>> c,  vector<double> f, vector<int> atende, vector<double> d);

bool verifica_factivel_restrito(vector<int> vetor_facilidade, vector<double> pen );

void clientes_ordenados(int m, int n, vector<vector<double>> c, vector<vector<int>> &matriz_clientes_ordenados);

void facilidades_ordenadas(int m, int n, vector<vector<double>> c, vector<vector<int>> &matriz_facilidades_ordenadas);

void ordena_clientes_decrescente_demanda( vector<int>&vetor_clientes, vector<double> d);

bool verifica_factivel( vector<double> pen );

void imprime_lista(list<int> lista);

int verifica_lista_tabu_facilidades (vector<vector<int>> lista_tabu_facilidades, vector<int> vetor_facilidades_proximas1, int Q, int tamanho_lista_tabu_facilidades, int ultima_melhora);

void acrescenta_lista_tabu_facilidades (vector<vector<int>> &lista_tabu_facilidades, vector<int> cluster, int Q, int tamanho_lista_tabu_facilidades);

int verifica_lista_tabu_clientes (vector<vector<int>> lista_tabu_clientes, vector<int> vetor_conjunto_clientes, int Q, int tamanho_lista_tabu_clientes, int ultima_melhora);

void acrescenta_lista_tabu_clientes (vector<vector<int>> &lista_tabu_clientes, vector<int> vetor_conjunto_clientes, int tamanho_lista_clientes);

void movimentos_fecha_abre_apos_solucao_inicial(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> &abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, list<int> &fechadas_disponiveis, double menor_demanda, double soma_das_demandas, vector<int> melhor_atende_relaxacao);

//void movimentos_fecha_abre_apos_solucao_inicial_best_improvement(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> &abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, list<int> &fechadas_disponiveis, double menor_demanda, double soma_das_demandas, vector<int> &novas_facilidades,  vector<int> facilidades_potenciais);

void movimentos_abre_apos_solucao_inicial_best_improvement(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> &abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, list<int> &fechadas_disponiveis, double menor_demanda, double soma_das_demandas, vector<int> &novas_facilidades, vector<int> &facilidades_potenciais, int quantidade_abertas_movimento1, vector<int> melhor_atende_relaxacao);

void movimentos_abre_apos_solucao_inicial(double &fo, int m, int n, vector<vector<double>> c, vector<vector<int>> matriz_facilidades_ordenadas, vector<int> vetor_clientes, vector<double> p, vector<double> d, bool &conseguiu_alocar, list<int> &abertas_disponiveis, vector<double> &pen,  vector<int> &atende, vector<double> f, list<int> &fechadas_disponiveis, double menor_demanda, double soma_das_demandas, vector<int> melhor_atende_relaxacao);

void Movement1 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas); 

void Movement1_restrito(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement1_restrito(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement1_restrito_factivel(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement1_restrito_factivel_first_improvement(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);


void Movement2realoca2 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas);

void Movement2realoca2_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement2realoca2_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement2realoca2_restrito_factivel(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void Movement2troca (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas);

void Movement2troca_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement2troca_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);
;

void explora_vizinhanca_Movement2troca_restrito_factivel (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement2troca_restrito_factivel_first_improvement (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void Movement3realoca3(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas);

void Movement3realoca3_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement3realoca3_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement3realoca3_restrito_factivel (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void Movement3triangulacao (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas);
;

void Movement3triangulacao_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis);

void explora_vizinhanca_Movement3triangulacao_restrito (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis) ;

void explora_vizinhanca_Movement3triangulacao_restrito_factivel(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis) ;

void explora_vizinhanca_movimento_fecha(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p);

void explora_vizinhanca_movimento_fecha2_first_improvement(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p, double menor_demanda);

void explora_vizinhanca_movimento_fecha2_best_improvement(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p, double menor_demanda);


void VND_factivel (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis) ;

void embaralhar( vector<int> &ordem, int numero_vizinhancas);

void VND_ils2 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p);

void VND_ils1 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<double> p);

void VND_factivel_1_2_3(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis, vector<double> p, double menor_demanda) ;

void VND_factivel_1_2_3_ils(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis, vector<double> p) ;

void best_improvement_movimento_abre(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int> vetor_clientes , bool factivel, list<int> abertas_disponiveis, vector<int> facilidadades_potenciais, vector<double> p, vector<vector<int>> &matriz_clientes_ordenados, double menor_demanda ) ;

void ILS_1 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>facilidades_potenciais, vector<double> p, int tamanho_instancia );

void ILS_1_novo(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>&facilidades_potenciais, vector<double> p, int tamanho_instancia, vector<vector<int>> matriz_clientes_ordenados, double menor_demanda, double soma_das_demandas, int ils_max1);

void ILS_1_novo2(int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>&facilidades_potenciais, vector<double> p, int tamanho_instancia, vector<vector<int>> matriz_clientes_ordenados, double menor_demanda, double soma_das_demandas, int ils_max1);

void ILS_2 (int m, int n, vector<list<int>>&s , double &fo, vector<double> &pen, vector<int> &atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> &abertas, list<int> &fechadas, vector<int>facilidades_potenciais, int tam_facilidades_potenciais, vector<double> p, int tamanho_instancia, int ils_max2, int constante_trocas);

void melhor_vizinho_movimento1 (int m, int n, double fo, vector<double> pen, vector<int> atende, vector<vector<double>> c, vector<double> d, vector<double> f, list<int> abertas, list<int> fechadas, int &cliente_realoca, int &facilidade_recebe, vector<list<int>> s, bool &melhorou);

void roda_relaxado_original (int m, int n, vector<double> f, vector<vector<double>> c, vector<double> d, vector<double> p, double soma_demandas, clock_t fim_CPU, clock_t inicio_CPU, double &tempo_relaxacao_no_raiz, double &limitante_inferior_no_raiz, int &quantidade_iteracoes, int &iteracao_que_encontrou_otimo, ofstream &arq_saida1, ofstream &arq_saida2, vector<list<int>>&corte1, vector<list<int>>&corte2, double &LB, double &primeiro_limitante_superior, double &otimo, vector<int> &atende, bool &factivel, vector<list<int>> &s, vector<double> &pen, list<int> &abertas, list<int> &fechadas, double &fo, list<int> &disponiveis_para_abrir, vector<vector<int>>&N );

void roda_relaxado_original2 (int m, int n, vector<double> f, vector<vector<double>> c, vector<double> d, vector<double> p, double soma_demandas, clock_t fim_CPU, clock_t inicio_CPU, double &tempo_relaxacao_no_raiz, double &limitante_inferior_no_raiz, int &quantidade_iteracoes, int &iteracao_que_encontrou_otimo, ofstream &arq_saida1, ofstream &arq_saida2, vector<list<int>>&corte1, vector<list<int>>&corte2, double &LB, double &primeiro_limitante_superior, double &otimo, vector<int> &atende, bool &factivel, vector<list<int>> &s, vector<double> &pen, list<int> &abertas, list<int> &fechadas, double &fo, list<int> &disponiveis_para_abrir,  vector<vector<int>>&N);

void constroi_N( int n, int m, vector<vector<double>> c , vector<vector<int>> &N, vector<double> d);

void verifica_se_problema_esparso_e_gap(list<int> disponiveis_para_abrir, bool &problema_esparso_e_gap, vector<int> vetor_conjunto_clientes, vector<double> p, double soma_demandas, vector<int> vetor_facilidades_proximas1, double &soma_capacidades);

void resolve_problema_esparso_e_atualiza_solucao1 (vector<int> vetor_facilidades_proximas1, int n, vector<double> d, vector<double> f, vector<double> p, vector<vector<double>> c, vector<int> &atende, bool coeficientes_inteiros,  double &UB, int &iteracao_que_encontrou_otimo, int &quantidade_iteracoes, ofstream &arq_saida1, ofstream &arq_saida2, vector<int> &melhor_atende, double &fo, bool &factivel, bool &UB_factivel );

void resolve_problema_esparso_e_atualiza_solucao2 (vector<int> vetor_facilidades_proximas1, int n, vector<double> d, vector<double> f, vector<double> p, vector<vector<double>> c, vector<int> &atende, bool coeficientes_inteiros,  double &UB, int &iteracao_que_encontrou_otimo, int &quantidade_iteracoes, ofstream &arq_saida1, ofstream &arq_saida2, vector<int> &melhor_atende, double &fo, bool &factivel, bool &UB_factivel );

void roda_matheuristica(int m, int n, vector<double> p,  list<int> disponiveis_para_abrir, vector<double> &pen, vector<list<int>> &s, vector<vector<double>> c, vector<double> f, vector<double> d, vector<int> &atende, bool coeficientes_inteiros, double &otimo, double &UB, list<int> &abertas, list<int> &fechadas, double &fo, double soma_demandas, bool imprime_detalhes, bool &factivel );

void monta_e_resolve_problema_denso (int m, int n, vector<double> f, vector<double> p, vector<vector<double>> c, vector<list<int>> &corte1, vector<list<int>> &corte2, double &LB, double soma_demandas, int &iter, vector<int> &atende, ofstream &arq_saida1, ofstream &arq_saida2, vector<double> d, bool &factivel, vector<list<int>> &s, vector<double> &pen, list<int> &abertas, list<int> &fechadas, double &fo, list<int> &disponiveis_para_abrir  );

void monta_e_resolve_problema_denso2 (int m, int n, vector<double> f, vector<double> p, vector<vector<double>> c, vector<list<int>> &corte1, vector<list<int>> &corte2, double &LB, double soma_demandas, int &iter, vector<int> &atende, ofstream &arq_saida1, ofstream &arq_saida2, vector<double> d, bool &factivel, vector<list<int>> &s, vector<double> &pen, list<int> &abertas, list<int> &fechadas, double &fo, list<int> &disponiveis_para_abrir  );



