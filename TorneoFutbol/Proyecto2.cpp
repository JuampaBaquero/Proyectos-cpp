#include <iostream>
#include <fstream>
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
    char grupo;
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
void eliminarEquipo(Torneo &, int);
void registrarPartido(Partido);
void actualizarJugador(Estadisticas, Jugador &);
void registrarJugador(Jugador, Equipo &);
void menuConsultas(Torneo &);
void archivoHistorico(Torneo &);
void cargar(int);

//Funciones de apoyo
bool leerEquipos(Torneo &torneo);
bool leerJugadores(Torneo &torneo);
bool leerPartidos(Torneo &torneo);

int main()
{
    Torneo torneo;
    int ingreso;

    cout << "+---------------- TORNEO COPA SUPREMA ----------------+" << endl
         << "|                                                     |" << endl
         << "|           ESTAMOS CARGANDO LOS ARCHIVOS             |" << endl
         << "+-----------------------------------------------------|" << endl;
    torneo = cargaDatos();
    cargar(3);
    while(ingreso != 7)
    {
        cout << "+---------------- TORNEO COPA SUPREMA ----------------+" << endl
             << "|   1) Incluir un equipo al torneo                    |" << endl
             << "|   2) Eliminar un equipo del torneo                  |" << endl
             << "|   3) Registrar un partido                           |" << endl
             << "|   4) Actualizar estadísticas de un jugador          |" << endl
             << "|   5) Registrar un nuevo jugador en el torneo        |" << endl
             << "|   6) Mostrar estadísticas de jugadores              |" << endl
             << "|   7) Salir.                                         |" << endl
             << "+-----------------------------------------------------+" << endl
             << "|--> ";
        cin >> ingreso;

        while(ingreso < 1 && ingreso > 7)
        {
            cout << "El dato ingresado es erróneo." << endl
                 << "Ingreselo nuevamente -> ";
            cin >> ingreso;
        }
    }
}

Torneo cargaDatos()
{
    Torneo miTorneo;
    if(leerEquipos(miTorneo) || leerJugadores(miTorneo) || leerPartidos(miTorneo)) miTorneo.cargoBien = false;
    return miTorneo;
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

    flujoEquipos >> buffer;
    torneo.numEquipos = stoi(buffer);

    char grupoActual = 'A';

    for(int i = 0; i < torneo.numEquipos; i++)
    {
        torneo.equipos[i].grupo = grupoActual;
        grupoActual++; //Asigno los grupos

        flujoEquipos >> buffer;
        torneo.equipos[i].nombre = buffer;

        flujoEquipos >> buffer;
        torneo.equipos[i].numJugadores = stoi(buffer);

        torneo.totalJugadores += torneo.equipos[i].numJugadores;
    }          
    flujoEquipos.close();

    return false;
}
bool leerJugadores(Torneo &torneo)
{
    //equipos, nombre, num idiomas, idiomas,  nacionalidad, posicion, codigo, fecha nacimiento, dorsal, num logros, logros
    //Asumimos que esto está ordenado por equipos.
    // de la anterior función tengo el total de jugadores
    string buffer;
    ifstream flujoJugadores("jugadores.txt", ios::in);

    if(!flujoJugadores.is_open())
    {
        cout << "El archivo jugadores.txt no se ha podido abrir exitosamente.";
        return true;
    }

    int jugadorAct = 0;
    string equipoAnterior = "";
    for(int i = 0; i < torneo.totalJugadores; i++)
    {
        flujoJugadores >> buffer;
        int indiceEquipo = buscarEquipo(buffer, torneo); 
        if(equipoAnterior != buffer) jugadorAct = 0;
        equipoAnterior = buffer;

        flujoJugadores >> buffer;
        if(indiceEquipo == -1)
        {
            cout << "El jugador -> " << buffer << " no se le ha podido asignar un equipo.";
            continue;
        }  
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].nombre = buffer;

        flujoJugadores >> buffer;
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].numIdiomas = stoi(buffer);

        for(int j = 0; j < torneo.equipos[indiceEquipo].jugadores[jugadorAct].numIdiomas; j++)
        {
            //Si son 0 idiomas, esto no se ejecuta.
            flujoJugadores >> buffer;
            torneo.equipos[indiceEquipo].jugadores[jugadorAct].idiomas[j] = buffer; 
        }

        flujoJugadores >> buffer;
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].nacionalidad = buffer;

        flujoJugadores >> buffer;
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].posicion = buffer;

        flujoJugadores >> buffer;
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].codigo = stoi(buffer);

        flujoJugadores >> buffer;
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].fechaNacimiento = stoi(buffer);

        flujoJugadores >> buffer;
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].dorsal = stoi(buffer);

        flujoJugadores >> buffer;
        torneo.equipos[indiceEquipo].jugadores[jugadorAct].numLogros = stoi(buffer);

        for(int j = 0; j < torneo.equipos[indiceEquipo].jugadores[jugadorAct].numLogros; j++)
        {
            flujoJugadores >> buffer;
            char logroAct = torneo.equipos[indiceEquipo].jugadores[jugadorAct].logros[j];
            logroAct = buffer[0];
            // Cojo el primer caracter de la cadena de texto, por si está mal en el archivo

            if(logroAct != 'E' && logroAct != 'B' && logroAct != 'A') logroAct = 'I';
             //Indicador personal de que algo está mal
        }
        jugadorAct++;
    }
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

void incluirEquipo(Torneo &, Equipo)
{
    // Registrar nuevo equipo
    // Valida que no exista
    // Asigna automáticamente el grupo
}
void eliminarEquipo(Torneo &, int)
{
    // Antes de eliminar: verificar si ya jugó algún partido
    // Si ya: No eliminar y mostrar N partidos
    // Si el usuario quiere, mostrar info de los partidos
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
void cargar(int x)
{
    for(int i = 0; i < x; i++)
    {
        Sleep(300);
        cout << ". ";
    }
    cout << endl;
}
