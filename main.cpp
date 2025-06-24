#include <iostream>
using namespace std;

// Prototipos de funciones (luego se implementan en los otros .cpp)
void registrarVendedores();     // registro.cpp
void registrarVentas();         // ventas.cpp
void analizarVentas();          // analisis.cpp

int main() {
    int opcion;

    do {
        system("cls");
        cout << "======================================" << endl;
        cout << "    Sistema de Ventas - Ventas Rapidas SA" << endl;
        cout << "======================================" << endl;
        cout << "1. Registrar vendedores" << endl;
        cout << "2. Registrar ventas del dia" << endl;
        cout << "3. Analizar ventas" << endl;
        cout << "0. Salir" << endl;
        cout << "--------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
        case 1:
            registrarVendedores();
            break;
        case 2:
            registrarVentas();
            break;
        case 3:
            analizarVentas();
            break;
        case 0:
            cout << "Saliendo del sistema..." << endl;
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente." << endl;
            break;
        }

    } while (opcion != 0);

    return 0;
}

void analizarVentas() {}

