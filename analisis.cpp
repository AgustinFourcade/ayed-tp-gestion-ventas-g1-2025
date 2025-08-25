#include <iostream>
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

int buscarVendedor(Vendedor vendedores[], int cantidad, int codigoBuscado);

int buscarProducto(int productos[], int cantidad, int codigoBuscado);

int cargarVendedores(Vendedor vendedores[], int maxVendedores);

void procesarVentas(
    Vendedor vendedores[],
    int totalVendedores,
    float ventasPorVendedor[],
    float ventasPorSucursal[],
    int productos[],
    int cantidadVentas[],
    int &totalProductos
);

void obtenerVendedorMaximo(Vendedor vendedores[], float ventasPorVendedor[], int totalVendedores);

void obtenerSucursalMaxima(Vendedor vendedores[], float ventasPorSucursal[], int totalVendedores);

void rankearProductos(int productos[], int cantidadVentas[], int totalProductos);


int main() {
    Vendedor vendedores[MAX_VENDEDORES];
    float ventasPorVendedor[MAX_VENDEDORES] = {0};
    float ventasPorSucursal[MAX_VENDEDORES] = {0};

    // Cargar vendedores
    int totalVendedores = cargarVendedores(vendedores, MAX_VENDEDORES);

    // Arrays para ranking de productos
    int productos[MAX_PRODUCTOS] = {0};
    int cantidadVentas[MAX_PRODUCTOS] = {0};
    int totalProductos = 0;

    // Procesar ventas
    procesarVentas(vendedores, totalVendedores, ventasPorVendedor, ventasPorSucursal, productos,
                   cantidadVentas, totalProductos);


    // Vendedor que más vendió
    obtenerVendedorMaximo(vendedores, ventasPorVendedor, totalVendedores);

    // Sucursal que más vendió
    obtenerSucursalMaxima(vendedores, ventasPorSucursal, totalVendedores);

    // Ranking de productos (orden básico tipo burbuja)
    rankearProductos(productos, cantidadVentas, totalProductos);

    cout << "\nAnalisis de ventas finalizado correctamente." << endl;

    return 0;
}

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

int cargarVendedores(Vendedor vendedores[], int maxVendedores) {
    FILE *archivo = fopen("vendedores.dat", "rb");
    if (!archivo) {
        cout << "No se pudo abrir el archivo de vendedores." << endl;
        return 0;
    }

    int total = 0;
    while (total < maxVendedores && fread(&vendedores[total], sizeof(Vendedor), 1, archivo) == 1) {
        total++;
    }

    fclose(archivo);
    return total;
}

void procesarVentas(
    Vendedor vendedores[],
    int totalVendedores,
    float ventasPorVendedor[],
    float ventasPorSucursal[],
    int productos[],
    int cantidadVentas[],
    int &totalProductos
) {
    FILE *archivoVentas = fopen("ventas_diarias.dat", "rb");
    if (!archivoVentas) {
        cout << "\nNo se pudo abrir el archivo de ventas." << endl;
        return;
    }

    Venta venta;
    while (fread(&venta, sizeof(Venta), 1, archivoVentas) == 1) {
        int posVendedor = buscarVendedor(vendedores, totalVendedores, venta.codVendedor);
        if (posVendedor == -1) continue;

        // Acumular venta por vendedor
        ventasPorVendedor[posVendedor] += venta.monto;

        // Acumular por sucursal
        for (int i = 0; i < totalVendedores; i++) {
            if (strcmp(vendedores[i].sucursal, vendedores[posVendedor].sucursal) == 0) {
                ventasPorSucursal[i] += venta.monto;
                break;
            }
        }

        // Contar producto
        int posProducto = buscarProducto(productos, totalProductos, venta.codProducto);
        if (posProducto == -1 && totalProductos < MAX_PRODUCTOS) {
            productos[totalProductos] = venta.codProducto;
            cantidadVentas[totalProductos] = 1;
            totalProductos++;
        } else if (posProducto != -1) {
            cantidadVentas[posProducto]++;
        }
    }

    fclose(archivoVentas);
}

void obtenerVendedorMaximo(Vendedor vendedores[], float ventasPorVendedor[], int totalVendedores) {
    float maxMonto = 0;
    int posMaxVend = -1;
    for (int i = 0; i < totalVendedores; i++) {
        if (ventasPorVendedor[i] > maxMonto) {
            maxMonto = ventasPorVendedor[i];
            posMaxVend = i;
        }
    }

    if (posMaxVend != -1) {
        cout << "Vendedor que mas vendio: " << vendedores[posMaxVend].nombre
                << " - Monto total: $" << ventasPorVendedor[posMaxVend] << endl;
    }
}

void obtenerSucursalMaxima(Vendedor vendedores[], float ventasPorSucursal[], int totalVendedores) {
    float maxSucursal = 0;
    int posMaxSucursal = -1;

    for (int i = 0; i < totalVendedores; i++) {
        if (ventasPorSucursal[i] > maxSucursal) {
            maxSucursal = ventasPorSucursal[i];
            posMaxSucursal = i;
        }
    }

    if (posMaxSucursal != -1) {
        cout << "Sucursal que mas vendio: " << vendedores[posMaxSucursal].sucursal
                << " - Monto total: $" << ventasPorSucursal[posMaxSucursal] << endl;
    }
}


void rankearProductos(int productos[1000], int cantidadVentas[1000], int totalProductos) {
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

    cout << "\nRanking de productos mas vendidos:\n";
    for (int i = 0; i < totalProductos; i++) {
        cout << "Producto " << productos[i]
                << " - Veces vendido: " << cantidadVentas[i] << endl;
    }
}
