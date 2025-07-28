#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

struct Vendedor {
    int codigo; //debe ser unico
    char nombre[50];
    char sucursal[50];
};

struct Venta {
    int fecha; // AAAAMMDD
    int codVendedor;
    int codProducto;
    float monto;
};

// Máximos esperados
const int MAX_VENDEDORES = 100;
const int MAX_PRODUCTOS = 1000;

int buscarVendedor(Vendedor vendedores[], int cantidad, int codigoBuscado) {
    for (int i = 0; i < cantidad; i++) {
        if (vendedores[i].codigo == codigoBuscado) {
            return i;
        }
    }
    return -1;
}

int buscarProducto(int productos[], int cantidad, int codigoBuscado) {
    for (int i = 0; i < cantidad; i++) {
        if (productos[i] == codigoBuscado) {
            return i;
        }
    }
    return -1;
}

int main() {
    Vendedor vendedores[MAX_VENDEDORES];
    float ventasPorVendedor[MAX_VENDEDORES] = {0};
    char sucursales[MAX_VENDEDORES][50];
    float ventasPorSucursal[MAX_VENDEDORES] = {0};
    int totalVendedores = 0;

    // Cargar vendedores
    ifstream archVend("vendedores.dat", ios::binary);
    if (!archVend) {
        cout << "Error al abrir vendedores.dat" << endl;
        return 1;
    }

    while (archVend.read((char *) &vendedores[totalVendedores], sizeof(Vendedor))) {
        // Guardamos sucursal en array paralelo
        strcpy(sucursales[totalVendedores], vendedores[totalVendedores].sucursal);
        totalVendedores++;
        if (totalVendedores >= MAX_VENDEDORES) break;
    }
    archVend.close();

    // Arrays para ranking de productos
    int productos[MAX_PRODUCTOS];
    int cantidadVentas[MAX_PRODUCTOS];
    int totalProductos = 0;

    // Procesar ventas
    ifstream archVentas("ventas_diarias.dat", ios::binary);
    if (!archVentas) {
        cout << "Error al abrir ventas_diarias.dat" << endl;
        return 1;
    }

    Venta v;
    while (archVentas.read((char *) &v, sizeof(Venta))) {
        int posVendedor = buscarVendedor(vendedores, totalVendedores, v.codVendedor);
        if (posVendedor == -1) continue;

        // Acumular venta por vendedor
        ventasPorVendedor[posVendedor] += v.monto;

        // Acumular por sucursal
        for (int i = 0; i < totalVendedores; i++) {
            if (strcmp(vendedores[i].sucursal, vendedores[posVendedor].sucursal) == 0) {
                ventasPorSucursal[i] += v.monto;
                break; // solo una vez por sucursal
            }
        }

        // Contar producto
        int posProducto = buscarProducto(productos, totalProductos, v.codProducto);
        if (posProducto == -1 && totalProductos < MAX_PRODUCTOS) {
            productos[totalProductos] = v.codProducto;
            cantidadVentas[totalProductos] = 1;
            totalProductos++;
        } else if (posProducto != -1) {
            cantidadVentas[posProducto]++;
        }
    }
    archVentas.close();

    // Vendedor que más vendió
    float maxMonto = 0;
    int posMaxVend = -1;
    for (int i = 0; i < totalVendedores; i++) {
        if (ventasPorVendedor[i] > maxMonto) {
            maxMonto = ventasPorVendedor[i];
            posMaxVend = i;
        }
    }

    if (posMaxVend != -1) {
        cout << "Vendedor que más vendió: " << vendedores[posMaxVend].nombre
                << " - Monto total: $" << ventasPorVendedor[posMaxVend] << endl;
    }

    // Sucursal que más vendió
    float maxSucursal = 0;
    int posMaxSucursal = -1;
    for (int i = 0; i < totalVendedores; i++) {
        if (ventasPorSucursal[i] > maxSucursal) {
            maxSucursal = ventasPorSucursal[i];
            posMaxSucursal = i;
        }
    }

    if (posMaxSucursal != -1) {
        cout << "Sucursal que más vendió: " << sucursales[posMaxSucursal]
                << " - Monto total: $" << ventasPorSucursal[posMaxSucursal] << endl;
    }

    // Ranking de productos (orden básico tipo burbuja)
    for (int i = 0; i < totalProductos - 1; i++) {
        for (int j = i + 1; j < totalProductos; j++) {
            if (cantidadVentas[j] > cantidadVentas[i]) {
                // Intercambiar productos
                int tempProd = productos[i];
                productos[i] = productos[j];
                productos[j] = tempProd;

                int tempCant = cantidadVentas[i];
                cantidadVentas[i] = cantidadVentas[j];
                cantidadVentas[j] = tempCant;
            }
        }
    }

    cout << "Ranking de productos más vendidos:\n";
    for (int i = 0; i < totalProductos; i++) {
        cout << "Producto " << productos[i]
                << " - Veces vendido: " << cantidadVentas[i] << endl;
    }

    return 0;
}
