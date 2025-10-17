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
	int main() {

    // q0: mientras lea 0 o 1, avanza a la derecha.
    // q0: al encontrar '_' escribe '1', avanza a la derecha y pasa a q1.
    // q1: al ver '_' (el siguiente blanco), simplemente aceptar.
    //
    // Resultado: se agrega un '1' al final del número binario.

    vector<Transiciones> trs = {
        // CAMBIO: recorrer a la derecha sobre bits
        {"q0", '0', "q0", '0', DER},
        {"q0", '1', "q0", '1', DER},

        // CAMBIO: al llegar al blanco, escribir '1' y moverse a q1
        {"q0", '_', "q1", '1', DER},

        // CAMBIO: desde q1, al ver el siguiente blanco, aceptar
        {"q1", '_', "q_accept", '_', DER}
        // (No hacen falta más transiciones)
    };
	
	string entrada;
    cout << "Ingrese palabra (binario sin signo): ";
    cin >> entrada;

    string inicio = "q0";
    MaquinaTuring mt(inicio, trs, entrada);
    mt.ejecutar();
    return 0;
}

};

