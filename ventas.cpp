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

struct Vendedor {
    int codigo; //debe ser unico
    char nombre[50];
    char sucursal[50];
};

void registrarVentas(int cantidadAgregar, FILE *archivo);
bool existeVendedor(int codigo);

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
    }

    registrarVentas(cantidadAgregar, archivo);
}

void registrarVentas(int cantidadAgregar, FILE *archivo) {
    for (int i = 0; i < cantidadAgregar; ++i) {
        Venta venta;

        cout << "\nVenta #" << (i + 1) << endl;
        cout << "Ingrese fecha (AAAAMMDD): ";
        cin >> venta.fecha;

        cout << "Ingrese codigo de vendedor: ";
        cin >> venta.codVendedor;

        if (!existeVendedor(venta.codVendedor)) {
            cout << "El vendedor con codigo " << venta.codVendedor << " no existe. Intente de nuevo." << endl;
            --i; // Decrementar i para repetir el registro
            continue; // Saltar al siguiente ciclo
        }

        cout << "Ingrese codigo de producto: ";
        cin >> venta.codProducto;

        cout << "Ingrese monto de la venta: ";
        cin >> venta.monto;

        fwrite(&venta, sizeof(Venta), 1, archivo);
        cout << "Venta registrada." << endl;
    }

    fclose(archivo);
}

bool existeVendedor(int codigo) {
    FILE *archivo = fopen("vendedores.dat", "rb");
    if (!archivo) {
        cout << "No se pudo abrir el archivo de vendedores." << endl;
        return false;
    }

    Vendedor vendedor;
    while (fread(&vendedor, sizeof(Vendedor), 1, archivo)) {
        if (vendedor.codigo == codigo) {
            fclose(archivo);
            return true; // Vendedor encontrado
        }
    }

    fclose(archivo);
    return false; // Vendedor no encontrado
}
