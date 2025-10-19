#include <iostream>
#include <windows.h>

using namespace std;

struct Partido
{
    string local;
    string visitante;
    string sede;
    string arbitro;
    int fecha; //AAAAMMDD
    int golLocal;
    int golVisitantes;
};

struct Estadisticas
{
    int numGoles;
    int asistencias;
    int tarjetas;
    int minJugados;
};

struct Jugador
{
    Estadisticas estadisticas;
    string idiomas[3];
    char logros[5];
    string nombre;
    string nacionalidad;
    string posicion;
    int codigo;
    int fechaNacimiento; // DDMMAAAA
    int dorsal;
};

struct Equipo
{
    string nombre; // Nombre corto, por ejemplo, BRA, COL...
    int numJugadores;
    char grupo;
    Jugador jugadores[10];
};

struct Torneo{
    Equipo equipos[32];
};

void cargaDatos(Torneo &);
void incluirEquipo(Torneo &, Equipo);
void eliminarEquipo(Torneo &, int);
void registrarPartido(Partido);
void actualizarJugador(Estadisticas, Jugador &);
void registrarJugador(Jugador, Equipo &);
void menuConsultas(Torneo &);
void archivoHistorico(Torneo &);
void cargar(int);

int main()
{
    Torneo torneo;
    int ingreso;

    cout << "+---------------- TORNEO COPA SUPREMA ----------------+" << endl
         << "|                                                     |" << endl
         << "|           ESTAMOS CARGANDO LOS ARCHIVOS             |" << endl
         << "+-----------------------------------------------------|" << endl;
    cargaDatos(torneo);
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

        while(ingreso < 1 && ingreso > 7){
            cout << "El dato ingresado es erróneo." << endl
                 << "Ingreselo nuevamente -> ";
            cin >> ingreso;
        }
    }
}

void cargaDatos(Torneo &)
{
    // Lee el archivo equipos.txt
    // Lee el archivo jugadores.txt
    // Lee resultadosParciales.txt
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
