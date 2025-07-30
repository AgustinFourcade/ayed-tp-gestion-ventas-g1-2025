#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

const int MAX_VENDEDORES = 15;

struct Vendedor {
    int codigo; //debe ser unico
    char nombre[50];
    char sucursal[50];
};

int contarVendedores();
bool excedeLimiteVendedores(int cantidadActual, int cantidadAgregar);
void registrarVendedores(int cantidadActual, int cantidadAgregar);
bool existeVendedor(int codigo);

int main() {
    int cantidadActual = contarVendedores();
    int cantidadAgregar;

    cout << "\nCantidad actual de vendedores: " << cantidadActual << endl;
    cout << "Cuantos vendedores desea agregar? ";
    cin >> cantidadAgregar;

    if (excedeLimiteVendedores(cantidadActual, cantidadAgregar)) {
        registrarVendedores(cantidadActual, cantidadAgregar);
    }
}

// Función principal para registrar vendedores
void registrarVendedores(int cantidadActual, int cantidadAgregar) {
    FILE* archivo = fopen("vendedores.dat", "ab");
    if (archivo == nullptr) {
        cout << "No se pudo abrir el archivo." << endl;
        return;
    }

    for (int i = 0; i < cantidadAgregar; ++i) {
        Vendedor vendedor;

        cout << "\nVendedor #" << (cantidadActual + i + 1) << endl;

        // Codigo
        cout << "Ingrese codigo de vendedor: ";
        cin >> vendedor.codigo;
        cin.ignore(); // limpiar \n

        if (existeVendedor(vendedor.codigo)) {
            cout << "Ya hay un vendedor que tiene este codigo. Intente con otro." << endl;
            --i; // Decrementar i para repetir el registro
            continue; // Saltar al siguiente ciclo
        }

        // Nombre
        cout << "Ingrese nombre del vendedor: ";
        cin.getline(vendedor.nombre, 50);

        // Sucursal
        cout << "Ingrese nombre de la sucursal: ";
        cin.getline(vendedor.sucursal, 50);

        // Guardar en archivo
        fwrite(&vendedor, sizeof(Vendedor), 1, archivo);
        cout << "Vendedor registrado con exito.\n";
    }

    fclose(archivo);
}

// Cuenta la cantidad de vendedores que hay en el archivo
int contarVendedores() {
    FILE* archivo = fopen("vendedores.dat", "rb");
    if (!archivo) return 0;

    Vendedor vendedor;
    int contador = 0;

    while (fread(&vendedor, sizeof(Vendedor), 1, archivo)) {
        contador++;
    }

    fclose(archivo);

    return contador;
}

//Verifica si al agregar una cantidad de vendedores no se supera el limite
bool excedeLimiteVendedores(int cantidadActual, int cantidadAgregar) {
    if (cantidadActual + cantidadAgregar > MAX_VENDEDORES) {
        cout << "Supera el limite de " << MAX_VENDEDORES << " vendedores. Puede agregar hasta "
                << (MAX_VENDEDORES - cantidadActual) << "." << endl;
        return false;
    }
    return true;
}

// Verifica si un vendedor con el codigo especificado ya existe
bool existeVendedor(int codigo) {
    FILE* archivo = fopen("vendedores.dat", "rb");
    if (!archivo) {
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
