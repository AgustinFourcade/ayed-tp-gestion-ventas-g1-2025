#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

const int MAX_VENDEDORES = 15;

struct Vendedor {
    int codigo; //unico
    char nombre[50];
    char sucursal[50];
};

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


// Función principal para registrar vendedores
void registrarVendedores() {
    int cantidadActual = contarVendedores();
    int cantidadAgregar;

    cout << "\nCantidad actual de vendedores: " << cantidadActual << endl;
    cout << "Cuantos vendedores desea agregar? ";
    cin >> cantidadAgregar;

    if (cantidadActual + cantidadAgregar > MAX_VENDEDORES) {
        cout << "Supera el limite de " << MAX_VENDEDORES << " vendedores. Puede agregar hasta "
            << (MAX_VENDEDORES - cantidadActual) << "." << endl;
        return;
    }

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
