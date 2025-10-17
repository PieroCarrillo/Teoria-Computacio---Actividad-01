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
		while (true) {
			bool trE = false;
			for (const auto& t : transiciones) {
				if (t.estadoActual == estadoActual && t.simboloLectura == cinta[cabezal]) {
					cinta[cabezal] = t.simboloEscritura;
					estadoActual = t.estadoSiguiente;
					cabezal += (t.dirMov == DER) ? 1 : -1;
					trE = true;
					break;
				}
			}

			if (!trE) {
				// no hay transición -> se detiene (acepta/rechaza según estado)
				break;
			}
		}

		cout << "Estado final: " << estadoActual << endl;
		cout << "Cinta final: ";
		for (char c : cinta) cout << c;
		cout << endl;
	}
};

int main(int argc, char** argv) {
	// MT binaria: 2X+1 = (shift left) + 1
	// Estados: q0 (scan), q1 (paso atrás), q2 (append 0), q3 (turn 0->1), qA
	// (accept sin transiciones)

	vector<Transiciones> trs = {
		// q0: recorrer bits hasta el blanco
		{"q0", '0', "q0", '0', DER},
		{"q0", '1', "q0", '1', DER},
		{"q0", '_', "q1", '_', IZQ}, // al ver blanco, retrocede una celda

		// q1: estamos sobre el último bit; avanzar a la derecha al blanco
		{"q1", '0', "q2", '0', DER},
		{"q1", '1', "q2", '1', DER},

		// Nota: si la entrada estaba vacía q0 vería '_' y retrocedería a -1.
		// En esta versión asumimos que siempre hay al menos un bit.

		// q2: en blanco, escribir '0' (shift left) y moverse a la izquierda
		{"q2", '_', "q3", '0', IZQ},

		// q3: sobre ese '0' recién puesto, convertir a '1' (+1) y entrar a
		// estado final sin salidas
		{"q3", '0', "qA", '1', DER},
	};

	string entrada;
	cout << "Ingrese palabra (binario): ";
	cin >> entrada;

	MaquinaTuring mt("q0", trs, entrada);
	mt.ejecutar();

	return 0;
}