#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

struct Venta {
    int fecha; // AAAAMMDD
    int codVendedor;
    int codProducto;
    float monto;
};

void registrarVentas(int cantidadAgregar, FILE *archivo);

int main() {
    FILE *archivo = fopen("ventas_diarias.dat", "ab");
    if (!archivo) {
        cout << "\nNo se pudo abrir el archivo de ventas." << endl;
        return 0;
    }

    int cantidadAgregar;
    cout << "\nCuantas ventas desea cargar hoy? (max 1000): ";
    cin >> cantidadAgregar;

    if (cantidadAgregar > 1000) {
        cout << "No puede cargar mas de 1000 ventas por dia." << endl;
        fclose(archivo);
        return 0;
    } else {
        registrarVentas(cantidadAgregar, archivo);
    }
}

void registrarVentas(int cantidadAgregar, FILE *archivo) {
    for (int i = 0; i < cantidadAgregar; ++i) {
        Venta venta;

        cout << "\nVenta #" << (i + 1) << endl;
        cout << "Ingrese fecha (AAAAMMDD): ";
        cin >> venta.fecha;

        cout << "Ingrese codigo de vendedor: ";
        cin >> venta.codVendedor;

        cout << "Ingrese codigo de producto: ";
        cin >> venta.codProducto;

        cout << "Ingrese monto de la venta: ";
        cin >> venta.monto;

        fwrite(&venta, sizeof(Venta), 1, archivo);
        cout << "Venta registrada." << endl;
    }

    fclose(archivo);
}
