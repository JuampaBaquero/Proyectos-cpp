#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>

using namespace std;

/*
ESTRUCTRURA DE LOS ARCHIVOS
Todo está separado por espacios, no por comas, las comas solo es para la legibilidad

EQUIPOS
//num equipos
//nombre, numero de jugadores

JUGADORES
//equipos, nombre, num idiomas, idiomas, nacionalidad, posicion, codigo, fecha nacimiento, dorsal, num logros, logros

PARTIDO
num partidos
//local, visitante, sede, arbitro, fecha, goles local, goles visitantes, numJugadores estadística
// -> por jugador: equipo, nombre, numgoles, asistencias, tarjetas, minJugados

EL MAYOR NUMERO DE PARTIDOS, ES DE 63, PORQUE EN FASE DE GRUPOS SE JUEGAN 6 X 8 PARTIDOS
MÁS LOS DE ELIMINACIÓN DIRECTA, QUE SON 15, PARA QUE SE ELIMINE UN PARTIDO POR EQUIPO
*/

struct Partido
{
    string local;
    string visitante;
    string sede;
    string arbitro;
    int numJugCambiados;
    int fecha; //AAAAMMDD
    int golLocal;
    int golVisitantes;
};

struct Estadisticas
{
    int numGoles = 0;
    int asistencias= 0;
    int tarjetas = 0;
    int minJugados = 0;
};

struct Jugador
{
    Estadisticas estadisticas;
    string idiomas[3];
    string nombre;
    string nacionalidad;
    string posicion;
    int numIdiomas;
    int numLogros;
    int codigo;
    int fechaNacimiento; // DDMMAAAA
    int dorsal;
    char logros[5];
};

struct Equipo
{
    Jugador jugadores[10];
    string nombre; // Nombre corto, por ejemplo, BRA, COL...
    int numJugadores;
    char grupo; //Es d (dieciseisavos), o (octavos), c (cuartos), s (semis), f (final) ((minúsculas))
};

struct Torneo{
    Equipo equipos[32];
    Partido partidos[63];
    int numEquipos;
    int totalJugadores;
    int numPartidos;
    bool cargoBien;
};

Torneo cargaDatos();
void incluirEquipo(Torneo &, Equipo);
void eliminarEquipo(Torneo &, string);
void registrarPartido(Partido);
void actualizarJugador(Estadisticas, Jugador &);
void registrarJugador(Jugador, Equipo &);
void menuConsultas(Torneo &);
void archivoHistorico(Torneo &);

//Funciones de apoyo
bool leerEquipos(Torneo &torneo);
bool leerJugadores(Torneo &torneo);
bool leerPartidos(Torneo &torneo);
void jugarPartido(Torneo &torneo, int);

void mostrarTodoXD(Torneo &);
int imprimirMenu();


int main()
{
    Torneo torneo;
    int ingreso = 1;

    cout << "+---------------- TORNEO COPA SUPREMA ----------------+" << endl
         << "|                                                     |" << endl
         << "|           ESTAMOS CARGANDO LOS ARCHIVOS             |" << endl
         << "+-----------------------------------------------------|" << endl;
    torneo = cargaDatos();
    Sleep(2000); //Espera dos segundos

    while(ingreso != 7)
    {
        ingreso = imprimirMenu();
        switch (ingreso)
        {
        case 8:
            mostrarTodoXD(torneo);
            break;
        
        default:
            break;
        }
    }
    
}

void mostrarTodoXD(Torneo &torneo){
    for(int i = 0; i < torneo.numEquipos; i++)
    {
        cout << "Equipo: " << torneo.equipos[i].nombre << endl
             << "Numero de jugadores: " << torneo.equipos[i].numJugadores << endl
             << "Jugadores: \n";

        for(int j = 0; j < torneo.equipos[i].numJugadores; j++)
        {
            cout << "\t" << j + 1 << ") " << torneo.equipos[i].jugadores[j].nombre; 
        } 
    }
}

int imprimirMenu()
{

    int ingreso;
    system("cls");
    cout << "+---------------- TORNEO COPA SUPREMA ----------------+" << endl
            << "|   1) Incluir un equipo al torneo                    |" << endl
            << "|   2) Eliminar un equipo del torneo                  |" << endl
            << "|   3) Registrar un partido                           |" << endl
            << "|   4) Actualizar estadísticas de un jugador          |" << endl
            << "|   5) Registrar un nuevo jugador en el torneo        |" << endl
            << "|   6) Mostrar estadísticas de jugadores              |" << endl
            << "|   7) Salir.                                         |" << endl
            << "|   8) Mostrar todo xd                                |" << endl
            << "+-----------------------------------------------------+" << endl
            << "|--> ";
    cin >> ingreso;

    while(ingreso < 1 && ingreso > 8)
    {
        cout << "El dato ingresado es erróneo." << endl
                << "Ingreselo nuevamente -> ";
        cin >> ingreso;
    }
    return ingreso;
}

Torneo cargaDatos()
{
    Torneo miTorneo;
    if(leerEquipos(miTorneo) || leerJugadores(miTorneo) || leerPartidos(miTorneo)) miTorneo.cargoBien = false;
    return miTorneo;
}

char subirFase(char grupo)
{
    if(grupo == 'A' || 
       grupo == 'B' || 
       grupo == 'C' || 
       grupo == 'D') return 'd';
    if(grupo == 'd') return 'o';
    if(grupo == 'o') return 'c';
    if(grupo == 'c') return 's';
    if(grupo == 's') return 'f';
    if(grupo == 'f') return 'G';
}

int buscarEquipo(string buscar, Torneo torneo)
{
    for(int i = 0; i < torneo.numEquipos; i++)
    {
        if(torneo.equipos[i].nombre == buscar) return i;
    }
    return -1; //Por si falla
}

bool leerEquipos(Torneo &torneo)
{
//    EQUIPOS
//num equipos
//nombre, numero de jugadores
    string buffer;
    ifstream flujoEquipos("equipos.txt", ios::in);

    if(!flujoEquipos.is_open())
    {
        cout << "No se ha podido encontrar el archivo 'equipos.txt'";
        return true;
    }

    stringstream ss;

    // num equipos
    flujoEquipos >> buffer;
    ss.clear();
    ss.str(buffer);
    ss >> torneo.numEquipos;

    char grupoActual = 'A';

    for(int i = 0; i < torneo.numEquipos; i++)
    {
        torneo.equipos[i].grupo = grupoActual;
        grupoActual++;

        flujoEquipos >> buffer;
        torneo.equipos[i].nombre = buffer;

        flujoEquipos >> buffer;
        ss.clear();
        ss.str(buffer);
        ss >> torneo.equipos[i].numJugadores;

        torneo.totalJugadores += torneo.equipos[i].numJugadores;
    }

    flujoEquipos.close();
    return false;
}


bool leerJugadores(Torneo &torneo)
{
    // equipos, nombre, num idiomas, idiomas, nacionalidad, posicion, codigo,
    // fecha nacimiento, dorsal, num logros, logros
    string buffer;
    ifstream flujoJugadores("jugadores.txt", ios::in);

    if (!flujoJugadores.is_open()) {
        cout << "El archivo jugadores.txt no se ha podido abrir exitosamente.";
        return true;
    }

    int jugadorAct = 0;
    string equipoAnterior = "";
    stringstream ss;

    for (int i = 0; i < torneo.totalJugadores; i++) {

        // equipo
        flujoJugadores >> buffer;
        int indiceEquipo = buscarEquipo(buffer, torneo);
        if (equipoAnterior != buffer) jugadorAct = 0;
        equipoAnterior = buffer;

        // nombre
        flujoJugadores >> buffer;
        if (indiceEquipo == -1) {
            cout << "El jugador -> " << buffer << " no se le ha podido asignar un equipo.";
            continue;
        }

        Jugador jugadorActObj;
        jugadorActObj.nombre = buffer;

        // num idiomas
        flujoJugadores >> buffer;
        ss.clear();
        ss.str(buffer);
        ss >> jugadorActObj.numIdiomas;

        // idiomas
        for (int j = 0; j < jugadorActObj.numIdiomas; j++) {
            flujoJugadores >> buffer;
            jugadorActObj.idiomas[j] = buffer;
        }

        // nacionalidad
        flujoJugadores >> buffer;
        jugadorActObj.nacionalidad = buffer;

        // posicion
        flujoJugadores >> buffer;
        jugadorActObj.posicion = buffer;

        // codigo
        flujoJugadores >> buffer;
        ss.clear();
        ss.str(buffer);
        ss >> jugadorActObj.codigo;

        // fecha nacimiento
        flujoJugadores >> buffer;
        ss.clear();
        ss.str(buffer);
        ss >> jugadorActObj.fechaNacimiento;

        // dorsal
        flujoJugadores >> buffer;
        ss.clear();
        ss.str(buffer);
        ss >> jugadorActObj.dorsal;

        // num logros
        flujoJugadores >> buffer;
        ss.clear();
        ss.str(buffer);
        ss >> jugadorActObj.numLogros;

        // logros
        for (int j = 0; j < jugadorActObj.numLogros; j++) {
            flujoJugadores >> buffer;
            char logroAct = buffer[0];
            if (logroAct != 'E' && logroAct != 'B' && logroAct != 'A')
                logroAct = 'I'; // inválido

            jugadorActObj.logros[j] = logroAct;
        }

        torneo.equipos[indiceEquipo].jugadores[jugadorAct] = jugadorActObj;
        jugadorAct++;
    }

    flujoJugadores.close();
    return false;
}


bool leerPartidos(Torneo &torneo)
{

// num partidos
// local, visitante, sede, arbitro, fecha, goles local, goles visitantes, numJugadores estadística

    string buffer;
    ifstream flujoPartidos("partidos.txt", ios::in);

    flujoPartidos >> buffer;
    torneo.numPartidos = stoi(buffer);

    for(int i = 0; i < torneo.numPartidos; i++)
    {
        flujoPartidos >> buffer;
        if(buscarEquipo(buffer, torneo) == -1) 
        {
            cout << "El equipo local no existe o está mal registrado.\n";
            return true;
        } 
        torneo.partidos[i].local = buffer;
        
        flujoPartidos >> buffer;
        if(buscarEquipo(buffer, torneo) == -1)
        {
            cout << "El equipo visitante no existe o está mal registrado.\n";
            return true;
        } 
        torneo.partidos[i].visitante = buffer;

        flujoPartidos >> buffer;
        torneo.partidos[i].sede = buffer;
        
        flujoPartidos >> buffer;
        torneo.partidos[i].arbitro = buffer;

        flujoPartidos >> buffer;
        torneo.partidos[i].visitante = buffer;

        flujoPartidos >> buffer;
        torneo.partidos[i].fecha = stoi(buffer);

        flujoPartidos >> buffer;
        torneo.partidos[i].golLocal = stoi(buffer);

        flujoPartidos >> buffer;
        torneo.partidos[i].golVisitantes = stoi(buffer);

        flujoPartidos >> buffer;
        torneo.partidos[i].numJugCambiados = stoi(buffer);

        // -> por jugador: equipo, nombre, numgoles, asistencias, tarjetas, minJugados
        flujoPartidos >> buffer;
        int equipoAct = buscarEquipo(buffer, torneo); 
        flujoPartidos >> buffer;
        if(equipoAct == -1)
        {
            cout << "El jugador " << buffer << " parece no pertenecer a ningún equipo. Revise la sintaxis."; 
            return true;
        }

        int jugadorAct = -1;
        for(int j = 0; j < torneo.equipos[equipoAct].numJugadores; j++)
        {
            if(torneo.equipos[equipoAct].jugadores[j].nombre == buffer) jugadorAct = j;
        }
        if(jugadorAct == 1)
        {
            cout << "El jugador " << buffer << " no se ha encontrado en el equipo.";
            return true;
        }
        flujoPartidos >> buffer;
        torneo.equipos[equipoAct].jugadores[jugadorAct].estadisticas.numGoles += stoi(buffer);
        
        flujoPartidos >> buffer;
        torneo.equipos[equipoAct].jugadores[jugadorAct].estadisticas.asistencias += stoi(buffer);
        
        flujoPartidos >> buffer;
        torneo.equipos[equipoAct].jugadores[jugadorAct].estadisticas.tarjetas += stoi(buffer);
        
        flujoPartidos >> buffer;
        torneo.equipos[equipoAct].jugadores[jugadorAct].estadisticas.minJugados += stoi(buffer);
    }
    return false;
}
void jugarPartido(Torneo &torneo, int idPartido)
{
    int idLocal = buscarEquipo(torneo.partidos[idPartido].local, torneo);
    int idVis = buscarEquipo(torneo.partidos[idPartido].visitante, torneo);
    
    if(torneo.equipos[idLocal].grupo != torneo.equipos[idVis].grupo)
    {
        cout << "Los equipos " << torneo.partidos[idPartido].local << " y " << torneo.partidos[idPartido].visitante
             << " no se encuentran en la misma fase o grupo.";
        return; 
    }
    int dif = torneo.partidos[idPartido].golLocal - torneo.partidos[idPartido].golVisitantes; 
    
    //gana el visitante
    if(dif < 0) torneo.equipos[idVis].grupo = subirFase(torneo.equipos[idVis].grupo); 

    //gana el local
    else if(dif > 0) torneo.equipos[idLocal].grupo = subirFase(torneo.equipos[idLocal].grupo);

    
}

void incluirEquipo(Torneo &torneo, Equipo equipo)
{
    // Valida que no exista
    if(torneo.numEquipos > 32)
    {
        cout << "No se ha podido crear el equipo . . ."
             << "\nEquipos llenos.\n";
        return;
    }
    if(buscarEquipo(equipo.nombre, torneo) != -1)
    {
        cout << "El equipo ya existe o tiene nombre igual . . .";
        return;
    }
    // Registrar nuevo equipo
    torneo.equipos[torneo.numEquipos + 1] = equipo; 

    // Asigna automáticamente el grupo
    char grupoAct = torneo.equipos[torneo.numEquipos].grupo;
    torneo.equipos[torneo.numEquipos + 1].grupo = grupoAct++;
}
void eliminarEquipo(Torneo &torneo, string nomEquipo)
{
    int idEncontrado;
    // Antes de eliminar: verificar si ya jugó algún partido
    for(int i = 0; i < torneo.numPartidos; i++)
    {
        if(torneo.partidos[i].local == nomEquipo)
        {
            char opc;
            idEncontrado = i;
            cout << "No se puede borrar el equipo.\n" 
                 << "Ya ha jugado partidos.\n\n"
                 << "--> Si quieres ver qué partido se encontró, ingresa una 'S' (si no, cualquier otra cosa): ";
            cin >> opc;
            
            if(opc == 'S' || opc == 's')
            {
                cout << "Fecha: " << torneo.partidos[idEncontrado].fecha << endl
                     << "Local: " << torneo.partidos[idEncontrado].local << endl
                     << "Visitante: " << torneo.partidos[idEncontrado].visitante << endl
                     << "Arbitro: " << torneo.partidos[idEncontrado].arbitro << endl
                     << "Resultado partido: (" << torneo.partidos[idEncontrado].golLocal 
                     << ", " << torneo.partidos[idEncontrado].golVisitantes << ")" << endl
                     << "Sede: " << torneo.partidos[idEncontrado].sede << endl;
            }
            return;
        }
    }
    Equipo equipoVacio;
    torneo.equipos[buscarEquipo(nomEquipo, torneo)] = equipoVacio;
}
void registrarPartido(Partido)
{

    // Validar que ambos equipos existan
    // Validar que ambos estén en la misma fase
    // Si más de 3 rojas -> notificar al comité disciplinario
}
void actualizarJugador(Estadisticas, Jugador &)
{
    // Verificar valores coherentes como: no más de 90 mins, o goles negativos
    // Si acumula 3 amarillas o tiene una roja, suspenderlo
}
void registrarJugador(Jugador, Equipo &)
{
    //Mirar lo de los trofeos, el char
}

void menuConsultas(Torneo &)
{
    int entrada;

    while(entrada != 5)
    {
// 1. Jugadores con al menos un gol en un partido específico
// 2. Numero de partidos jugados en un equipo en una fase determinada
// 3. Jugador con más goles por fase (mostrar empates)
// 4. Equipos donde hay jugadores que nacieron luego del 2000, y tienen nivel E en inglés
    }
}

void archivoHistorico(Torneo &)
{
    //eq | codJug | jug | gol | asist | tarj | min | susp (si/no)
}
