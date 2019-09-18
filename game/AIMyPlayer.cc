#include "Player.hh"

#define PLAYER_NAME agus97_v3


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  static constexpr int I[8] = { 1, 1, 0, -1, -1, -1,  0,  1 };
  static constexpr int J[8] = { 0, 1, 1,  1,  0, -1, -1, -1 };

  typedef vector<int> VE;


  bool fuego(int ip, int jp) {
    int count = 0;
    int count1 = 0;
    for(int i = ip-2; i<= ip+2; ++i){
      for(int j = jp-2; j <= jp+2; ++j){

          int id_soldado = which_soldier(i,j);
          if((id_soldado>0) and data(id_soldado).player!=me())  ++count1;
          if((id_soldado>0) and data(id_soldado).player==me())  ++count;
      }
    }
    if(count1>=count+3) return true;
    else return false;
}

    bool no_chocar (int i ,int j){
        int id_helicoptero = which_helicopter(i,j);
        for(int x = -4; x <= 4;++x ){
            int land = what(i+x,j+5);
            int id_helicoptero = which_helicopter(i,j);
            if(id_helicoptero>0) return false;
        }
        return true;
    }

  bool terreno_valido_soldado(int i, int j) {
      int land;
      if(not pos_ok(i,j)) return false;
      else{
         land = what(i,j);
         int id_soldado = which_soldier(i,j);
         if(land==WATER or land==MOUNTAIN or fire_time(i,j)!=0 or ((id_soldado>0) and (data(id_soldado).player==me()))){
             return false;
         }
         else {
             return true;
        }
      }
  }

  pair<int,int> puedo_tirar(int i,int j){
      pair<int,int> retornar;
      for(int x=-2;x<=2;++x){
          for(int y=-2; y<=2; ++y){
              int land = what(i+x,j+y);
              int poste = post_owner(i+x,j+y);
              if(which_soldier(i+x,j+y)==0 and (poste>=0 or poste==-1) and fire_time(i+x,j+y)){
                  retornar.first=i+x;
                  retornar.second=j+y;
                  return retornar;
              }
          }

      }
      retornar.first=-1;
      retornar.second=-1;
      return retornar;
}




  bool terreno_valido_soldado2(int i, int j) {
      int land;
      if(not pos_ok(i,j)) return false;
      else{
         land = what(i,j);
         int id_helicoptero = which_helicopter(i,j);
         if(land==MOUNTAIN or fire_time(i,j)!=0 or ((id_helicoptero>0) and (data(id_helicoptero).player==me()))){
             return false;
         }
         else {
             return true;
        }
      }
  }

  bool terreno_valido_helicoptero(int ip, int jp) {
      int land;
      if(not no_chocar(ip,jp)) return false;
      for(int i = ip-2; i< ip+2; ++i){
        for(int j = jp-2; j < jp+2; ++j){
            land = what(i,j);
            int id_helicoptero = which_helicopter(i,j);
            if(not pos_ok(i,j)) return false;
            if(land==MOUNTAIN or ((id_helicoptero>0) and (data(id_helicoptero).player!=me())) ) return false;
        }
      }
      return true;
  }

  pair<int ,int > BFS(int ipos,int jpos){
      auto p = make_pair(false, false);
      vector<vector<pair<bool,bool> > > visita(MAX, vector<pair<bool, bool>> (MAX, p)); //1r VISITAT , 2n DINS
      vector<vector<pair<int ,int> >> direccion(MAX,vector<pair<int ,int>> (MAX));

      queue<pair<int,int>> Q;

      visita[ipos][jpos].first= true;
      visita[ipos][jpos].second=true;
      for(int i = 0; i < 8; ++i){
          if(terreno_valido_soldado(ipos+I[i],jpos+J[i])){ //puede ir
              pair<int,int> adyacente;
              adyacente.first = ipos+I[i];
              adyacente.second = jpos+J[i];
              direccion[ipos+I[i]][jpos+J[i]]=adyacente;
              Q.push(adyacente);
              visita[ipos+I[i]][jpos+J[i]].second=true;
          }
          else { //no entren mĂĄs
               visita[ipos+I[i]][jpos+J[i]].first=true;
               visita[ipos+I[i]][jpos+J[i]].second=true;
          }
      }

      while(not Q.empty()){
          pair<int,int> v = Q.front(); //cogemos los de la lista
          int ip = v.first; //guardamos i del 1r de la cola
          int jp = v.second;//guardamos j del 1r de la cola
          Q.pop(); //Nos cargamos el primer numero de la cola
          if(not visita[ip][jp].first){ //no he visitado la posicion
              visita[ip][jp].first= true; //posicio segur que esta be, nomĂŠs poso a la cua les correctes
              int post = post_owner(ip,jp);
              if(terreno_valido_soldado(ip,jp) and (post==-1 or (post>=0 and post!=me()))) {
                  return direccion[ip][jp];
              }
              if(terreno_valido_soldado(ip,jp)){
                  int countdos = 0;
                  for(int i = 0; i < 8; ++i){
                          if(terreno_valido_soldado(ip+I[countdos],jp+J[countdos]) and not visita[ip+I[countdos]][jp+J[countdos]].first and not visita[ip+I[countdos]][jp+J[countdos]].second){
                              direccion[ip+I[countdos]][jp+J[countdos]]=direccion[ip][jp];
                              pair<int,int> adyacente;
                              adyacente.first = ip+I[countdos];
                              adyacente.second = jp+J[countdos];
                              Q.push(adyacente);
                              visita[ip+I[countdos]][jp+J[countdos]].second = true;
                              int post = post_owner(ip+I[countdos],jp+J[countdos]);
                          }
                          else if(pos_ok(ip+I[countdos],jp+J[countdos])) {
                              visita[ip+I[countdos]][jp+J[countdos]].second = true;
                          }
                    ++countdos;
                }
            }
        }
    }
  }

  void play_soldier(int id) {
    Data in = data(id);
    int i = in.pos.i;
    int j = in.pos.j;
    int player = in.player;
    bool atacat = false;

    for (int x = 0; x < 8; ++x) {
        pair<int,int> posicio;
        posicio.first = i + I[x];
        posicio.second = j + J[x];
        int id_soldier = which_soldier(posicio.first, posicio.second);
        if (id_soldier > 0 and data(id_soldier).player != player) {
            command_soldier(id, posicio.first, posicio.second);
            atacat = true;
        }
    }
    if (not atacat ) {
        pair<int,int> resultado = BFS(i,j);
        command_soldier(id, resultado.first, resultado.second);
    }
  }

  void play_helicopter() {
      int helicopteros_id = me();
      vector<int> heli = helicopters(helicopteros_id);
      int numero_helicopteros = heli.size();
      double q = status(me());
      for(int i = 0; i < numero_helicopteros; ++i) {
          //if( status(me())>=q+double(1)) return;
          int rondas = round();
          int pro = me();
          Data helic = data(heli[i]);
          int numero_paracaidistas = helic.parachuters.size();
          int i_heli = helic.pos.i;
          int j_heli = helic.pos.j;
          for (int x = 0; x < numero_paracaidistas; x++) throw_parachuter(heli[i],q);
          if (data(heli[i]).napalm == 0 and fuego(i_heli,j_heli) and (fuego(i_heli,j_heli))) {
              command_helicopter(heli[i], NAPALM);
          }
          if (rondas == 0) {
              if (helicopteros_id == 0){
                  if (helic.orientation == 0) command_helicopter(heli[i], COUNTER_CLOCKWISE);
                  else command_helicopter(heli[i], CLOCKWISE);
              }
              else if (helicopteros_id == 1){
                  if (helic.orientation == 0) command_helicopter(heli[i], CLOCKWISE);
                  else command_helicopter(heli[i], COUNTER_CLOCKWISE);
              }
              else if (helicopteros_id == 2){
                  if (helic.orientation == 2) command_helicopter(heli[i], COUNTER_CLOCKWISE);
                  else command_helicopter(heli[i], CLOCKWISE);
              }
              else if (helicopteros_id == 3){
                  if (helic.orientation == 2) command_helicopter(heli[i], CLOCKWISE);
                  else command_helicopter(heli[i], COUNTER_CLOCKWISE);
              }
          }
            else if (rondas > 1 and (rondas%40 == 0 or rondas%40 == 1) ) command_helicopter(heli[i], CLOCKWISE);
            else command_helicopter(heli[i], FORWARD2);
        }
      }

  void throw_parachuter(int helicopter_id,double q) {
    // We get the data of the helicopter...
    Data in = data(helicopter_id);
    int count=0;
    // ... and try to throw a parachuter, without even examining the land.
    pair<int,int> tirar=puedo_tirar(in.pos.i,in.pos.j);
    if(tirar.first!=-1) command_parachuter(tirar.first, tirar.second);
    else{
        tirar.first = in.pos.i + random(-2, 2);
        tirar.second = in.pos.j + random(-2, 2);
        while (tirar.first >= 0 and tirar.first < MAX and tirar.second >= 0 and tirar.second < MAX)  {
             ++count;
             if(count>=300) return;
             tirar.first = in.pos.i + random(-2, 2);
             tirar.second = in.pos.j + random(-2, 2);
            int terra = what(tirar.first,tirar.second);
            if (terra != MOUNTAIN and terra != WATER and which_soldier(tirar.first,tirar.second) == 0 and fire_time(tirar.first,tirar.second)==0){
                command_parachuter(tirar.first, tirar.second);
                return;
            }
        }
    }
}

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    int player = me();
    VE S =    soldiers(player); //    soldiers of my player
    play_helicopter();
    for (int i = 0; i < (int)S.size(); ++i) play_soldier(S[i]);
  }

};

constexpr int PLAYER_NAME::I[8];
constexpr int PLAYER_NAME::J[8];

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
