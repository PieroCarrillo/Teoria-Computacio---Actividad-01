#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Direccion { IZQ, DER };

struct Transiciones {
	string estadoActual;
	char simboloLectura;
	string estadoSiguiente;
	char simboloEscritura;
	Direccion dirMov;
};

class MaquinaTuring {
	string estadoActual;
	int cabezal;
	vector<char> cinta;
	vector<Transiciones> transiciones;

public:
	MaquinaTuring(string estadoInicial, vector<Transiciones> tr, string entrada)
		: estadoActual(estadoInicial), transiciones(tr), cabezal(0) {
		cinta = vector<char>(entrada.begin(), entrada.end());
		cinta.push_back('_'); // blanco
	}

	    void ejecutar() {
        // CAMBIO: detener cuando lleguemos al estado de aceptación "q_accept"
        while (estadoActual != "q_accept") {
            bool trE = false;
            for (const auto& t : transiciones) {
                if (t.estadoActual == estadoActual && t.simboloLectura == cinta[cabezal]) {
                    cinta[cabezal] = t.simboloEscritura;
                    estadoActual = t.estadoSiguiente;
                    cabezal += (t.dirMov == DER) ? 1 : -1;

                    // CAMBIO (robusto): si nos movemos a la izquierda fuera de índice, agregar blanco al inicio
                    if (cabezal < 0) {
                        cinta.insert(cinta.begin(), '_');
                        cabezal = 0;
                    }
                    // CAMBIO (robusto): si nos movemos más allá del final, agregar blanco
                    if (cabezal >= (int)cinta.size()) {
                        cinta.push_back('_');
                    }

                    trE = true;
                    break;
                }
            }
            if (!trE) break; // si no hay transición, se detiene (seguridad)
        }
         cout << "Estado final: " << estadoActual << endl;

        // CAMBIO: imprimir la cinta : solo hasta el primer '_'
        cout << "Cinta final: ";
        for (char c : cinta) {
            if (c == '_') break;
            cout << c;
        }
        cout << endl;
    
    };

};

