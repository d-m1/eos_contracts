#include <eosio/eosio.hpp>
#include <eosio/singleton.hpp>
#include <eosio/system.hpp>

#include <stdexcept>

using namespace eosio;
using namespace std;

class [[eosio::contract("hemerton")]] hemerton : public eosio::contract {

public:
  
  hemerton(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds), total(_self, _self.value) {}

  [[eosio::action]]
  void open(name user) {
    require_auth( user );
    index default_ind;
    auto results = total.get_or_create(_self, default_ind);
    uint32_t request_index = results.total_requests + 1;
    data_index table( get_self(), get_first_receiver().value );
    table.emplace(user, [&]( auto& row ) {
      row.key = request_index;
      row.user = user;
      row.data = "Empty";
    });
    total.set(index{request_index}, user);
  }

  [[eosio::action]]
  void erase(int request, name user) {
    require_auth(user);

    data_index table( get_self(), get_first_receiver().value);

    auto iterator = table.find(request);
    check(iterator != table.end(), "Record does not exist");
    table.erase(iterator);
  }

  [[eosio::action]]
  void getindex() {
    auto result = total.get();
    print(result.total_requests);
  }

  [[eosio::action]]
  void upload(int request, name user, std::string data) {
    require_auth(user);

    data_index table( get_self(), get_first_receiver().value);

    auto iterator = table.find(request);
    check(iterator != table.end(), "Record does not exist");
    table.modify(iterator, user, [&]( auto& row ) {
        row.data = data;
    });
  }

  long double factorial(int n, int n2) {
    long double res=1;
    if (n2!=0){
        for(int i=n;i<=n2;i++)
        {
            res=res*i;   
        }
    }
    return res;
  }

  [[eosio::action]]
  void smlist(name user, vector<int> &testing, int x, int y) {
    print("helou");
    print("||||||");
    vector<vector< int> > b;
    b.resize(x);
    if (x*y != testing.size())
    {
        print("NO CUADRA");
    }
    for (int i = 0; i < x; i++)
    {
        b[i].resize(y);
    }
    int aux = 0;
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            b[i][j] = testing[aux];
            aux = aux + 1;
        }
    }
    for(unsigned int i=0; i < x; i++) {
        for(unsigned int j=0; j<y; j++) {
            print(b[i][j]);
            print(" ");
        }
        print("\n");
    }
    print("\n||||||     ");
    print(current_time_point().sec_since_epoch());
  }


  [[eosio::action]]
  int testing(name user, 
              int nAccMax, int durMaxAcc, int durPru, 
              int sRes, 
              vector<int> &nTAT, int nAT,
              vector<int> &vNAccR, 
              vector<int> &vNR,
              vector<int> action_p_action_p_restriction,
              int x_axis, int y_axis
              ) {
    
    print("TESTING | ");
    print(current_time_point().sec_since_epoch());
    if (x_axis*y_axis != action_p_action_p_restriction.size())
    {
        print("Parametros de matriz incorrectos");
        return 0;
    } else
    {
      // Mapeo Tipos de acciones por accion por cada restriccion
      vector<vector<int>> vtAR;
      vtAR.resize(x_axis);
      for (int i = 0; i < x_axis; i++)
      {
          vtAR[i].resize(y_axis);  
      }
      int aux = 0;
      for (int i = 0; i < x_axis; i++)
      {
          for (int j = 0; j < y_axis; j++)
          {
              vtAR[i][j] = action_p_action_p_restriction[aux];
              aux++;
          }
      }
    
      //NÚMERO DE RESTRICCIONES
      int nR=sizeof(vNAccR)/sizeof(int);
      //NÚMERO DE RESTRICCIONES CON REPETICION
      int nRR=0;
      for(int i=0;i<nR;i++){
          nRR=nRR+vNR[i];
      }
      //SEGUNDOS RESTANTES
      int sRs=durPru-(nAccMax*durMaxAcc);
      //TOTAL DE ACCIONES RESTRICCIONES
      int tAR=0;
      for(int i=0;i<nR;i++){
          tAR=tAR+vNR[i]*vNAccR[i];
      }
      //ACCIONES SUELTAS
      int aS=(nAccMax-tAR);
      //TOTAL DE ELEMENTOS
      int tE=sRs+nRR+aS;
      //CALCULAMOS LA ENTROPIA DEL ESQUELETO DEL SISTEMA
      long double entropia;
      entropia=factorial(sRs+1,tE);
      for (int i=0;i<nR;i++){
          entropia=entropia/factorial(1,vNR[i]);
      }
      entropia=entropia/factorial(1,aS);
      
      /*-------------------
      GENERACION DE NÚMERO ALEATORIO-1 DE 1 A ENTROPIA DEL ESQUELETO
      --------------------*/
      int ale1=1;

      //METEMOS EN UN VECTOR LAS REPETICIONES
      int vRes[nR+2];
      int n;
      if (nR!=0){
          for(int i=0;i<nR;i++){
              vRes[i]=vNR[i];
              n=i+1;
          }
      }

      if (aS!=0){
          vRes[n]=aS;
          
      }else{
          vRes[n]=0;
      }
      n++;

      if (sRs!=0){
          vRes[n]=sRs;
      }else{
          vRes[n]=0;
      }

      //HAYAMOS LA MATRIZ DE INICIO DE ACCIONES POR CADA RESTRICCIÓN, EL ORDEN DEL CONJUNTO ES RESTRICCIONES, ACCIONES SUELTAS Y SEGUNDOS
      long double lim1;
      long double lim2;
      int iAcc[tE][3];
      //printf("%i",tE);
      
    
      int vRes2[nR+2];
      for(int i=0;i<=n;i++){
          vRes2[i]=vRes[i];
      }

      lim1=0;
    
      for(int i=0;i<tE;i++){
          for(int j=0;j<=n;j++){
              if (vRes2[j]>0){
                  lim2=(entropia/(tE-i+1))*vRes2[j];
                  lim2=lim2+lim1;
                  if((lim1<ale1)&&(ale1<=lim2)){
                      vRes2[j]=vRes2[j]-1;
                      iAcc[i][1]=j+1;
                      j=nR+2;
                  }else{
                      lim1=lim2;
                  }
              }  
          }
      }

    
      //MATRIZ INICIO ELEMENTOS Y TIPOS

      int mIT[nAccMax+sRs][4];
      int n1=0;

      for(int i=1;i<=nR;i++){
          for(int k=0;k<tE;k++){
              if (iAcc[k][1]==i){
                  iAcc[k][2]=vNAccR[i-1]*durMaxAcc;
                  iAcc[k][3]=vNAccR[i-1];
                  for(int j=0;j<vNAccR[i-1];j++){
                      mIT[n1][0]=durMaxAcc;
                      mIT[n1][1]=vtAR[i-1][j];
                
                      if(n1<tE){
                          n1++;
                      }
                  }
              }
          } 
      }

      for(int i=nR+1;i<=aS+nR;i++){
          for(int k=0;k<tE;k++){
              if (iAcc[k][1]==i){
                  iAcc[k][2]=durMaxAcc;
                  iAcc[k][3]=1;
                  mIT[n1][0]=durMaxAcc;
                  mIT[n1][1]=0;
              
                  if(n1<tE){
                      n1++;
                  }
              }
          }
      }

      for(int i=aS+nR;i<tE;i++){
          for(int k=0;k<tE;k++){
              if (iAcc[k][1]==i){
                  iAcc[k][2]=1;
                  iAcc[k][3]=0;
                  mIT[n1][0]=1;
                  mIT[n1][1]=-1;
                  if(n1<nAccMax+sRs){
                      n1++;
                  }
              }
          }
      }

      int seg=0;
      mIT[0][3]=0;
      for(int i=1;i<nAccMax+sRs;i++){
          mIT[i][2]=mIT[i-1][0]+mIT[i-1][2];
      }

      //CÁLCULO ENTROPIA ACCIONES SOBRE EL ESQUELETE TENIENDO EN CUENTA LOS TIPOS, T1,T2...PARA RESTRICCIONES
      //TODAS LAS ACCIONES POSIBLES PARA LAS ACCIONES SUELTAS
      long double entropia2=1;

      
      //VECTOR ACCION CONCRETA POR CADA ELEMENTO
      int vACE[nAccMax][3];
      int m=0;
      //NÚMERO DE ACCIONES TOTALES POSIBLES
      int nAccTP=0;
      for(int i=0;i<(sizeof(nTAT)/sizeof(int));i++){
          nAccTP=nAccTP+nTAT[i];
      }

      for(int i=1;i<=(sizeof(nTAT)/sizeof(int));i++){
          for(int j=0;j<nAccMax+sRs;j++){
              if (mIT[j][1]==i){
                  entropia2=entropia2*nTAT[i-1];
                  mIT[j][2]= nTAT[i-1];
              }  
          }
      }

      for(int j=0;j<nAccMax+sRs;j++){
          if (mIT[j][1]==0){
              entropia2=entropia2*nAccTP;
              mIT[j][2]=nAccTP;
          }else if(mIT[j][1]==-1){
              mIT[j][2]=0;
          }
      }
      
      



      /*------------------
      GENERACIÓN ALEATORIO2 ENTRE 1 Y ENTROPIA2
      ----------------*/
      
      int ale2;

      ale2=90;
      //printf("%i",mIT[0][2]);
      lim1=0;
      long double entropia3=entropia2;
      for(int i=0;i<nAccMax+sRs;i++){
          for(int j=1;j<=mIT[i][2];j++){
              //printf("%i",mIT[j][2]);
              if (mIT[i][2]!=0){
                  lim1=(entropia3/mIT[i][2])*j;
                  if(ale2<lim1){
                      mIT[i][3]=j;
                    
                      
                      ale2=ale2-((j-1)*mIT[i][2]);
              
                      j=100;
                      entropia3=(entropia3/mIT[i][2]);
                  }
              }else{
                  mIT[i][3]=0;
              
              }
              
          }
      }
      print(ale2);
      
      for(int i=1;i<nAccMax+sRs;i++){
          mIT[i][0]=mIT[i][0]+mIT[i-1][0];
      }
      
      
      /*----------------------
      MATRIZ DE RESULTADOS mIT, 
      Columna 0, final en el segundos de cada elemento, elementos son acciones o los proprios segundos 
      Columna 1, Tipos de acción alfanuméricos, -1 si no hay acción en ese rango (para segundos vacios)
      Columna 2, Número de acciones por tipo en cada acción de reestricción o totales en caso de acciones sueltas, 0 para segundos
      Columna 3, Número de elemento elegido de la lista, ya sea para lista completa o lista restringida por cada tipo

      --------------------*/
      
      
      
      return 1;
    }
  }

private:
  struct [[eosio::table]] data {
    int key;
    name user;
    std::string data;
    uint64_t primary_key() const { return key; }
  };
  typedef eosio::multi_index<"proofs"_n, data> data_index;

  struct index {
    uint32_t total_requests = 0;
  };
  typedef eosio::singleton<"index"_n, index> index_table;

  index_table total;

};