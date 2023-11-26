#include <Windows.h>
#include "resource.h"
#include <string>
#include <cstdio>
#include <iostream>
#include <commdlg.h>
#include <wingdi.h>
#include <fstream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cstdlib>  // Necesario para la función rand
#include <ctime>
#pragma once


using namespace std;
int selectedIndex = -1;

//Fechas
struct NodoFecha {
	int dia;
	int mes;
	int anno;

	// Constructor predeterminado
	NodoFecha() : dia(0), mes(0), anno(0) {}

	// Constructor que toma los datos como parámetros
	NodoFecha(int d, int m, int a) : dia(d), mes(m), anno(a) {}
};

// Datos de los Eventos
NodoFecha fechas[36];

const char* meses[50] = { "Todos", "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};
const char* descuento[50] = {"2 x 1", "10 %"};

//Nodo Eventos
struct NodoEvento {
    char nombre[50] = { 0 };
    NodoFecha fecha;
    int disponibles = 0;
    int cantidadInicial = 0;
    float costoInicial = 0;
    float costoFinal = 0;
    NodoEvento* siguienteEvento;
    NodoEvento* anteriorEvento;
    int indice; // Nuevo campo para almacenar el índice en el array eventos

    NodoEvento() : siguienteEvento(nullptr), anteriorEvento(nullptr), indice(-1) {}

    // Constructor que toma los datos como parámetros
    NodoEvento(const char* n, const NodoFecha& f, int disp, int cantIni, float costoIni, float costoFin, int ind)
        : fecha(f), disponibles(disp), cantidadInicial(cantIni), costoInicial(costoIni), costoFinal(costoFin),
        siguienteEvento(nullptr), anteriorEvento(nullptr), indice(ind) {
        strncpy_s(nombre, n, sizeof(nombre) - 1);
    }
};

NodoEvento eventos[36];

typedef NodoEvento* EventoPuntero;

EventoPuntero actualEvento = nullptr;
EventoPuntero temporalEvento = nullptr;
EventoPuntero cabezaEvento = nullptr;

//Nodo Precios
struct NodoPrecio {
	EventoPuntero evento;
	float costoTotal;
	NodoPrecio* siguientePrecio;
	NodoPrecio* anteriorPrecio;

	// Constructor
	NodoPrecio(EventoPuntero e, float costo)
		: evento(e), costoTotal(costo), siguientePrecio(nullptr), anteriorPrecio(nullptr) {}
};

typedef NodoPrecio* PrecioPuntero;

PrecioPuntero cabezaPrecio = nullptr;
PrecioPuntero temporalPrecio = nullptr;

//Nodo Usuarios
struct NodoUsuario {
	char usuario[50] = { 0 };
	char contraseña[50] = { 0 };
	char nombre[50] = { 0 };
	NodoUsuario* siguienteUsuario;
	NodoUsuario* anteriorUsuario;

	// Vector de punteros a eventos comprados
	vector<EventoPuntero> eventosComprados;

	// Constructor
	NodoUsuario(const char* nom, const char* pass, const char* user)
		: siguienteUsuario(nullptr), anteriorUsuario(nullptr) {
		strncpy_s(usuario, user, sizeof(usuario) - 1);
		strncpy_s(contraseña, pass, sizeof(contraseña) - 1);
		strncpy_s(nombre, nom, sizeof(nombre) - 1);
	}
};

typedef NodoUsuario* UsuarioPuntero;

// Nodo UsuarioEvento
struct NodoPrecioUsuario {
	PrecioPuntero NodoPrecio;
	UsuarioPuntero usuario;
	int boletos;  // Nuevo campo
	NodoPrecioUsuario* siguientePrecioUsuario;
	NodoPrecioUsuario* anteriorPrecioUsuario;

	// Constructor
	NodoPrecioUsuario(PrecioPuntero precio, UsuarioPuntero user, int b)
		: NodoPrecio(precio), usuario(user), boletos(b), siguientePrecioUsuario(nullptr), anteriorPrecioUsuario(nullptr) {}
};


typedef NodoPrecioUsuario* PrecioUsuarioPuntero;

PrecioUsuarioPuntero cabezaPrecioUsuario = nullptr;
PrecioUsuarioPuntero temporalPrecioUsuario = nullptr;

UsuarioPuntero actualUsuario = nullptr;
UsuarioPuntero temporalUsuario = nullptr;
UsuarioPuntero cabezaUsuario = nullptr;
UsuarioPuntero nuevoUsuario = nullptr;
vector<NodoEvento*> vectorTemporal;
vector<NodoEvento*> vectorDisponibles;

HINSTANCE hInst;                                // current instance
int elegidos = 0;

//Funciones
void inicializarNodos();
int obtenerNumeroAleatorio(int min, int max);
bool compararPorFecha(const EventoPuntero& evento1, const EventoPuntero& evento2);
bool usuarioYaRegistrado(const char* nombre);

LRESULT CALLBACK VentanaInicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaCalendario(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaRegistrarse(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaComprar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaDatos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaMisCompras(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hwnd;
	MSG mensaje;

	hwnd = CreateDialog(hInstance, MAKEINTRESOURCE(Pantalla_InicioSesion), NULL, VentanaInicioSesion);
	ShowWindow(hwnd, SW_SHOWDEFAULT);

	HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	while (TRUE == GetMessage(&mensaje, 0, 0, 0))
	{
		TranslateMessage(&mensaje);
		DispatchMessage(&mensaje);
	}

	return mensaje.wParam;
}

LRESULT CALLBACK VentanaInicioSesion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg)
	{
	//INITIAL
	case WM_INITDIALOG: {
		inicializarNodos();
		//Imprimir usuario ya guardado
		cabezaEvento;
		SetDlgItemTextA(hwnd, Edit_UsuarioExistente, cabezaUsuario->usuario);
		SetDlgItemTextA(hwnd, Edit_ContrasenaExistente, cabezaUsuario->contraseña);
	}break;

	//COMMAND
	case WM_COMMAND: {
		if (LOWORD(wParam) == Boton_IniciarSesion_InicioSesion) {
			//Validacion de datos

			//Obtener los valores del editbox
			char usuario[50] = { 0 };
			char contrasena[50] = { 0 };
			GetDlgItemTextA(hwnd, Edit_Usuario_InicioSesion, usuario, sizeof(usuario));
			GetDlgItemTextA(hwnd, Edit_Contrasena_InicioSesion, contrasena, sizeof(contrasena));

			//Inicializar el temporalUsuario
			temporalUsuario = cabezaUsuario;

			//Si está vacío (No debería ya que esta inicializado)
			if (temporalUsuario == nullptr) {
				MessageBox(hwnd, "No hay usuarios ingresados", "Error", MB_OK | MB_ICONERROR);
			}

			//Ciclo para ir comparando los usuarios
			while (temporalUsuario != nullptr) {
				//Buscar usuario
				if (strcmp(usuario, temporalUsuario->usuario) == 0) {
					//Buscar contraseña
					if (strcmp(contrasena, temporalUsuario->contraseña) == 0) {
						EndDialog(hwnd, 0);
						actualUsuario = temporalUsuario;
						DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
						break;
					}
					else {
						MessageBox(hwnd, "Contrasena incorrecta", "Error", MB_OK | MB_ICONERROR);
						break;
					}
				}
				//Buscar a traves de la lista
				temporalUsuario = temporalUsuario->siguienteUsuario;
				if (temporalUsuario == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "Error", MB_OK | MB_ICONERROR);
					break;
				}
			}//Fin del ciclo

		}
		else if (LOWORD(wParam) == Boton_Registrarse_InicioSesion) {
			//Crear Ventana de Registro
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Registrarse), hwnd, VentanaRegistrarse);
		}
		else if (LOWORD(wParam) == Datos) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Datos), hwnd, VentanaDatos);
		}
		else if (LOWORD(wParam) == Calendario) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
		}
		else if (LOWORD(wParam) == MisCompras) {

		}
		else if (LOWORD(wParam) == 40004) {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}

	} break;

	//CLOSE
	case WM_CLOSE: {
		DestroyWindow(hwnd);
		PostQuitMessage(0);
	}break;

	}
	return 0;
}

LRESULT CALLBACK VentanaRegistrarse(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	char usuario[50] = { 0 };
	char contrasena[50] = { 0 };
	char nombre[50] = { 0 };

	switch (msg)
	{
		//INITIAL
	case WM_INITDIALOG: {


	}break;

		//COMMAND
	case WM_COMMAND: {
		if (LOWORD(wParam) == Boton_Registrar_Registrarse) {
			
			//Guardar los datos de los edit en varibales
			GetDlgItemTextA(hwnd, Edit_Usuario_Registrar, usuario, sizeof(usuario));
			GetDlgItemTextA(hwnd, Edit_Contrasena_Registrar, contrasena, sizeof(contrasena));
			GetDlgItemTextA(hwnd, Edit_Contrasena_Registrar2, nombre, sizeof(nombre));

			//Validar que no esten vacíos
			if (strlen(nombre) == 0 || strlen(usuario) == 0 || strlen(contrasena) == 0) {
				MessageBox(hwnd, "Por favor, complete todos los campos.", "Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			//Validar que el nombre no tenga letras
			for (size_t i = 0; i < strlen(nombre); ++i) {
				if (!isalpha(nombre[i]) && nombre[i] != ' ') {
					MessageBox(hwnd, "El nombre debe contener solo letras.", "Error", MB_OK | MB_ICONERROR);
					return 0;
				}
			}

			if (usuarioYaRegistrado(usuario)) {
				MessageBox(hwnd, "Este usuario ya está registrado.", "Error", MB_OK | MB_ICONERROR);
				delete nuevoUsuario;  // Cleanup allocated memory
				return 0;  // Abort registration
			}

			//Copiar los valores a un nuevo nodo
			nuevoUsuario = new NodoUsuario(nombre, contrasena, usuario);

			// Insertar nodo
			if (cabezaUsuario == nullptr) {
				nuevoUsuario->siguienteUsuario = nullptr;
				cabezaUsuario = nuevoUsuario;
			}
			else {
				// Caso: Insertar al final de la lista
				UsuarioPuntero actual = cabezaUsuario;
				while (actual->siguienteUsuario != nullptr) {
					actual = actual->siguienteUsuario;
				}
				nuevoUsuario->siguienteUsuario = nullptr;
				actual->siguienteUsuario = nuevoUsuario;
			}


			MessageBox(hwnd, "Se a registrado con exito.", "Éxito", MB_OK | MB_ICONINFORMATION);
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Pantalla_InicioSesion), NULL, VentanaInicioSesion);

		}
		else if (LOWORD(wParam) == Boton_Volver_Registrarse) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Pantalla_InicioSesion), hwnd, VentanaInicioSesion);
		}
		else if (LOWORD(wParam) == Datos) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Datos), hwnd, VentanaDatos);
		}
		else if (LOWORD(wParam) == Calendario) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
		}
		else if (LOWORD(wParam) == MisCompras) {
			DialogBox(hInst, MAKEINTRESOURCE(Lista_Eventos_MisCompras), hwnd, VentanaMisCompras);

		}
		else if (LOWORD(wParam) == 40004) {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
	} break;

		//CLOSE
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
		DialogBox(hInst, MAKEINTRESOURCE(Pantalla_InicioSesion), hwnd, VentanaInicioSesion);
	}break;

	}
	return 0;


}

LRESULT CALLBACK VentanaCalendario(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	HWND comboBoxEvento = GetDlgItem(hwnd, Combo_Mes_Eventos);

	const int MAX_NOMBRE_EVENTO = 50;
	char nombreSeleccionado[MAX_NOMBRE_EVENTO] = { 0 };
	NodoFecha fechaSeleccionada;
	int disponiblesSeleccionados = 0;
	float costoInicialSeleccionado = 0.0;
	float costoFinalSeleccionado = 0.0;

	int lenghtMes = GetWindowTextLength(comboBoxEvento);
	char mes[100];
	char contenedor[50];
	int indice = 0;

	switch (msg)
	{
		//INITIAL
	case WM_INITDIALOG: {
		SendDlgItemMessageA(hwnd, List_EventosMes_Eventos, LB_RESETCONTENT, 0, 0);

		temporalEvento = cabezaEvento;
		while (temporalEvento != nullptr) {
			temporalEvento->indice = -1;
			temporalEvento = temporalEvento->siguienteEvento;
		}

		//Poner datos en el combobox
		for (int i = 0; i < 13; i++) {
			SendMessage(comboBoxEvento, CB_ADDSTRING, NULL, (LPARAM)meses[i]);
		}

		//Poner "todos" en el combobox
		SendMessage(comboBoxEvento, CB_SETCURSEL, 0, 0);

		// Ordenar el vector temporal por fecha y hora
		sort(vectorTemporal.begin(), vectorTemporal.end(), [](const NodoEvento* cita1, const NodoEvento* cita2) {
			// Comparar primero por año, luego por mes y finalmente por día
			if (cita1->fecha.anno != cita2->fecha.anno) {
				return cita1->fecha.anno < cita2->fecha.anno;
			}
			else if (cita1->fecha.mes != cita2->fecha.mes) {
				return cita1->fecha.mes < cita2->fecha.mes;
			}
			else {
				return cita1->fecha.dia < cita2->fecha.dia;
			}

		});

		// Mostrar las citas ordenadas en el ListBox
		for (const auto& cita : vectorTemporal) {
			string infoCita = "Cita " + string(cita->nombre) + ": " + to_string(cita->fecha.dia) + "/" + to_string(cita->fecha.mes) + "/" + to_string(cita->fecha.anno);
			SendDlgItemMessageA(hwnd, List_EventosMes_Eventos, LB_ADDSTRING, 0, (LPARAM)infoCita.c_str());

			cita->indice = indice;

			// Verificar si temporalEvento no es nullptr antes de acceder a su índice
			if (temporalEvento != nullptr) {
				temporalEvento->indice = indice;
				temporalEvento = temporalEvento->siguienteEvento;
			}

			indice++; // Incrementar el índice para el próximo nodo
		}

	}break;

		//COMMAND
	case WM_COMMAND: {
		if (LOWORD(wParam) == Boton_VisualizarEvento_Evento) {
			int index = SendDlgItemMessage(hwnd, List_EventosMes_Eventos, LB_GETCURSEL, 0, 0);

			if (index != LB_ERR) {
				// Buscar el evento correspondiente usando el índice
				temporalEvento = cabezaEvento;
				while (temporalEvento != nullptr) {
					if (temporalEvento->indice == index) {
						SetDlgItemTextA(hwnd, Edit_Titulo_Eventos, temporalEvento->nombre);
						char fechaStr[20];
						sprintf_s(fechaStr, sizeof(fechaStr), "%d/%d/%d", temporalEvento->fecha.dia, temporalEvento->fecha.mes, temporalEvento->fecha.anno);

						char disponiblesStr[10];
						sprintf_s(disponiblesStr, sizeof(disponiblesStr), "%d", temporalEvento->disponibles);

						char costoInicialStr[20];
						sprintf_s(costoInicialStr, sizeof(costoInicialStr), "%.2f", temporalEvento->costoInicial);

						// Mostrar los valores en los controles correspondientes
						SetDlgItemTextA(hwnd, Edit_Fecha_Eventos, fechaStr);
						SetDlgItemTextA(hwnd, Edit_Disponibles_Evento, disponiblesStr);
						SetDlgItemTextA(hwnd, Edit_Costo_Eventos, costoInicialStr);
						break;  // ¡Importante detener el bucle cuando se encuentra el evento!
					}
					temporalEvento = temporalEvento->siguienteEvento;
				}
			}

		}
		
		else if(LOWORD(wParam) == Boton_Comprar_Evento) {
			int index = SendDlgItemMessage(hwnd, List_EventosMes_Eventos, LB_GETCURSEL, 0, 0);

			if (index != LB_ERR) {
				// Buscar el evento correspondiente usando el índice
				temporalEvento = cabezaEvento;
				while (temporalEvento != nullptr) {
					if (temporalEvento->indice == index) {
						actualEvento = temporalEvento;
						break;  // ¡Importante detener el bucle cuando se encuentra el evento!
					}
					temporalEvento = temporalEvento->siguienteEvento;
				}
			}

			actualEvento = temporalEvento;
			
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Ventas), hwnd, VentanaComprar);
		}

		else if (LOWORD(wParam) == Boton_Filtrar_Eventos) {

			// Almacena el nuevo índice seleccionado	
			GetWindowText(comboBoxEvento, contenedor, 50);

			// Comparar contenedor con el mes
			int selectedIndex = -1;  // Inicializar selectedIndex
			for (int i = 0; i < 13; i++) {
				if (strcmp(contenedor, meses[i]) == 0) {
					selectedIndex = i;  // Asignar el índice seleccionado
					break;
				}
			}

			temporalEvento = cabezaEvento;

			// Reiniciar los índices
			int indice = 0;

			while (temporalEvento != nullptr) {
				temporalEvento->indice = -1;
				temporalEvento = temporalEvento->siguienteEvento;
			}

			// Filtrar eventos según el mes seleccionado
			vector<NodoEvento*> eventosFiltrados;
			temporalEvento = cabezaEvento;

			while (temporalEvento != nullptr) {
				if (selectedIndex == 0 || strcmp(contenedor, meses[temporalEvento->fecha.mes]) == 0) {
					eventosFiltrados.push_back(temporalEvento);
				}
				temporalEvento = temporalEvento->siguienteEvento;
			}

			// Limpiar el ListBox
			SendDlgItemMessageA(hwnd, List_EventosMes_Eventos, LB_RESETCONTENT, 0, 0);

			// Ordenar eventos filtrados por fecha
			sort(eventosFiltrados.begin(), eventosFiltrados.end(), [](const NodoEvento* evento1, const NodoEvento* evento2) {
				// Comparar primero por año, luego por mes y finalmente por día
				if (evento1->fecha.anno != evento2->fecha.anno) {
					return evento1->fecha.anno < evento2->fecha.anno;
				}
				else if (evento1->fecha.mes != evento2->fecha.mes) {
					return evento1->fecha.mes < evento2->fecha.mes;
				}
				else {
					return evento1->fecha.dia < evento2->fecha.dia;
				}
				});

			// Mostrar las citas ordenadas en el ListBox
			for (const auto& cita : eventosFiltrados) {
				string infoCita = "Cita " + string(cita->nombre) + ": " + to_string(cita->fecha.dia) + "/" + to_string(cita->fecha.mes) + "/" + to_string(cita->fecha.anno);
				SendDlgItemMessageA(hwnd, List_EventosMes_Eventos, LB_ADDSTRING, 0, (LPARAM)infoCita.c_str());
				cita->indice = indice;
				indice++;
			}
		}

		else if (LOWORD(wParam) == Datos) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Datos), hwnd, VentanaDatos);
		}
		else if (LOWORD(wParam) == Calendario) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
		}
		else if (LOWORD(wParam) == MisCompras) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_MisCompras), hwnd, VentanaMisCompras);
		}
		else if (LOWORD(wParam) == 40004) {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
		
	} break;

		//CLOSE
	case WM_CLOSE: {
		DestroyWindow(hwnd);
		PostQuitMessage(0);
	}break;

	}
	return 0;


}

LRESULT CALLBACK VentanaComprar(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	HWND hcomboBoxDisp = GetDlgItem(hwnd, Combo_CantidadBoletos_Venta);
	HWND hcomboBoxDesc = GetDlgItem(hwnd, Combo_Descuentos_Venta);
	char contenedor[50];
	char contenedorDescuento[50];
	float precio;

	switch (msg)
	{
		//INITIAL
	case WM_INITDIALOG: {

		//Escribir nombre y fecha
		char fechaStr[20];

		if (actualEvento != nullptr) {
			sprintf_s(fechaStr, sizeof(fechaStr), "%d/%d/%d", actualEvento->fecha.dia, actualEvento->fecha.mes, actualEvento->fecha.anno);
			// Rest of the code that uses actualEvento
		}
		else {
			return 0;
		}
		string costo = to_string(actualEvento->costoInicial);

		SetDlgItemTextA(hwnd, Edit_Titulo_Ventas, actualEvento->nombre);
		SetDlgItemTextA(hwnd, Edit_CostoIndividual_Ventas, costo.c_str());
		SetDlgItemTextA(hwnd, Edit_Fecha_Ventas, fechaStr);

		//Combobox Disponibles
		int disponiblesCheck = actualEvento->disponibles;

		//Vector de disponibles - Ponerlos en el Combobox 

		vector<int> disponiblesVector;
		for (int i = 1; i <= disponiblesCheck; ++i) {
			disponiblesVector.push_back(i);
		}

		for (int i = 0; i < disponiblesCheck; i++) {
			string strDisponible = to_string(disponiblesVector[i]);
			SendMessage(GetDlgItem(hwnd, Combo_CantidadBoletos_Venta), CB_ADDSTRING, 0, (LPARAM)strDisponible.c_str());
		}

		//Combobox Descuento
		for (int i = 0; i < 2; i++) {
			SendMessage(hcomboBoxDesc, CB_ADDSTRING, NULL, (LPARAM)descuento[i]);
		}

	}break;

		//COMMAND
	case WM_COMMAND: {
		if (LOWORD(wParam) == Boton_Calcular_Eventos) {
			//Obtener cuantos quiere y calcular total
			GetWindowText(hcomboBoxDisp, contenedor, 50);
			elegidos = atoi(contenedor);
			actualEvento->disponibles = actualEvento->disponibles - elegidos;

			if (elegidos == 0) {
				MessageBox(hwnd, "Favor de elegir la cantidad de boletos", "Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			GetWindowText(hcomboBoxDesc, contenedorDescuento, 50);

			int elegidoDescuento = atoi(contenedorDescuento);

			if (strcmp(contenedorDescuento, "2 x 1") == 0) {
				if (elegidos % 2 == 0) {
					elegidos = elegidos / 2;
					precio = actualEvento->costoInicial * elegidos;
					elegidos = elegidos * 2;
				}
				else {
					elegidos--;
					elegidos = elegidos / 2;
					elegidos++;
					precio = actualEvento->costoInicial * elegidos;
					elegidos = (elegidos * 2) - 1;
				}
			}
			else if (strcmp(contenedorDescuento, "10 %") == 0) {
				float totalCosto = actualEvento->costoInicial * elegidos;
				totalCosto = totalCosto - (0.1 * totalCosto);
				precio = totalCosto;
			}
			else {
				precio = actualEvento->costoInicial * elegidos;
			}

			actualEvento->disponibles = actualEvento->disponibles - elegidos;

			string precioStr = to_string(precio);
			SetDlgItemTextA(hwnd, Edit_Total_Ventas, precioStr.c_str());

		}
		else if (LOWORD(wParam) == Boton_Volver_Venta) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
		}
		else if (LOWORD(wParam) == Datos) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Datos), hwnd, VentanaDatos);
		}
		else if (LOWORD(wParam) == Calendario) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
		}
		else if (LOWORD(wParam) == MisCompras) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_MisCompras), hwnd, VentanaMisCompras);

		}
		else if (LOWORD(wParam) == 40004) {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}

		else if (LOWORD(wParam) == Boton_Comprar_Evento) {

			GetWindowText(hcomboBoxDesc, contenedorDescuento, 50);
			GetWindowText(hcomboBoxDisp, contenedor, 50);
			elegidos = atoi(contenedor);
			//Calcular

			if (elegidos == 0) {
				MessageBox(hwnd, "Favor de elegir la cantidad de boletos", "Error", MB_OK | MB_ICONERROR);
				return 0;
			}

			int elegidoDescuento = atoi(contenedorDescuento);

			if (strcmp(contenedorDescuento, "2 x 1") == 0) {
				if (elegidos % 2 == 0) {
					elegidos = elegidos / 2;
					precio = actualEvento->costoInicial * elegidos;
					elegidos = elegidos * 2;
				}
				else {
					elegidos--;
					elegidos = elegidos / 2;
					elegidos++;
					precio = actualEvento->costoInicial * elegidos;
					elegidos = (elegidos * 2) - 1;
				}
			}
			else if (strcmp(contenedorDescuento, "10 %") == 0) {
				float totalCosto = actualEvento->costoInicial * elegidos;
				totalCosto = totalCosto - (0.1 * totalCosto);
				precio = totalCosto;
			}
			else {
				precio = actualEvento->costoInicial * elegidos;
			}

			actualEvento->disponibles = actualEvento->disponibles - elegidos;

			string precioStr = to_string(precio);
			SetDlgItemTextA(hwnd, Edit_Total_Ventas, precioStr.c_str());

			// Crear un nuevo objeto NodoPrecio
			NodoPrecio* nuevoPrecio = new NodoPrecio(actualEvento, precio);
			NodoPrecioUsuario* nuevoPrecioUsuario = new NodoPrecioUsuario(nuevoPrecio, actualUsuario, elegidos);


			actualUsuario->eventosComprados.push_back(actualEvento);

			// Conectar en la lista de NodoPrecio
			if (cabezaPrecio == nullptr) {
				cabezaPrecio = nuevoPrecio;
			}
			else {
				nuevoPrecio->siguientePrecio = cabezaPrecio;
				cabezaPrecio->anteriorPrecio = nuevoPrecio;
				cabezaPrecio = nuevoPrecio;
			}


			// Conectar en la lista de NodoPrecioUsuario
			if (cabezaPrecioUsuario == nullptr) {
				cabezaPrecioUsuario = nuevoPrecioUsuario;
			}
			else {
				nuevoPrecioUsuario->siguientePrecioUsuario = cabezaPrecioUsuario;
				cabezaPrecioUsuario->anteriorPrecioUsuario = nuevoPrecioUsuario;
				cabezaPrecioUsuario = nuevoPrecioUsuario;
			}


			//Final
			MessageBox(hwnd, "Compra Existosa", "Exito", MB_OK | MB_ICONINFORMATION);
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);


		} break;
		//CLOSE
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
		DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
	}break;
	}
	}
				   return 0;
}

LRESULT CALLBACK VentanaDatos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
switch (msg)
{
	case WM_INITDIALOG: {
		SetDlgItemTextA(hwnd, Edit_Nombre_Datos, actualUsuario->nombre);
		SetDlgItemTextA(hwnd, Edit_Usuario_Datos, actualUsuario->usuario);
		SetDlgItemTextA(hwnd, Edit_Contrasena_Datos, actualUsuario->contraseña);
	}break;
		//COMMAND
	case WM_COMMAND: {
		if (LOWORD(wParam) == Datos) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Datos), hwnd, VentanaDatos);
		}
		else if (LOWORD(wParam) == Calendario) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
			}
		else if (LOWORD(wParam) == MisCompras) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_MisCompras), hwnd, VentanaMisCompras);
		}
		else if (LOWORD(wParam) == 40004) {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
	} break;

		//CLOSE
	case WM_CLOSE: {
		DestroyWindow(hwnd);		
		PostQuitMessage(0);
	}break;	
	}

	return 0;
}

LRESULT CALLBACK VentanaMisCompras(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	switch (msg)
	{
		//INITIAL
	case WM_INITDIALOG: {

		HWND hListBox = GetDlgItem(hwnd, Lista_Eventos_MisCompras);
		int index = -1;
		// Sort events based on the custom comparison function
		std::sort(actualUsuario->eventosComprados.begin(), actualUsuario->eventosComprados.end(), compararPorFecha);

		// Add sorted events to the list box
		for (const auto& evento : actualUsuario->eventosComprados) {
			char eventoStr[100];
			sprintf_s(eventoStr, sizeof(eventoStr), "%s - %d/%d/%d", evento->nombre, evento->fecha.dia, evento->fecha.mes, evento->fecha.anno);
			SendMessage(hListBox, LB_ADDSTRING, 0, (LPARAM)eventoStr);
			index++;
		}


	}break;

		//COMMAND
	case WM_COMMAND: {
		if (LOWORD(wParam) == Boton_Visualizar_Compra) {
			HWND hListBox = GetDlgItem(hwnd, Lista_Eventos_MisCompras);
			int selectedIndex = SendMessage(hListBox, LB_GETCURSEL, 0, 0);

			if (selectedIndex != LB_ERR) {
				NodoPrecioUsuario* selectedNodoPrecioUsuario = nullptr;
				int currentIndex = 0;
				NodoPrecioUsuario* currentNodoPrecioUsuario = cabezaPrecioUsuario;

				while (currentNodoPrecioUsuario != nullptr && currentIndex < selectedIndex) {
					currentNodoPrecioUsuario = currentNodoPrecioUsuario->siguientePrecioUsuario;
					currentIndex++;
				}

				selectedNodoPrecioUsuario = currentNodoPrecioUsuario;

				if (currentIndex == selectedIndex && selectedNodoPrecioUsuario != nullptr) {
					// Dereference pointers to get information
					PrecioPuntero selectedPrecio = selectedNodoPrecioUsuario->NodoPrecio;
					UsuarioPuntero selectedUsuario = selectedNodoPrecioUsuario->usuario;
					int boletosComprados = selectedNodoPrecioUsuario->boletos;

					// Now you can use the information to populate the dialog box
					SetDlgItemTextA(hwnd, Edit_Titulo_Compra, selectedPrecio->evento->nombre);

					char fechaStr[20];
					sprintf_s(fechaStr, sizeof(fechaStr), "%d/%d/%d", selectedPrecio->evento->fecha.dia, selectedPrecio->evento->fecha.mes, selectedPrecio->evento->fecha.anno);
					SetDlgItemTextA(hwnd, Edit_Fecha_Compra, fechaStr);

					float totalCosto = selectedPrecio->costoTotal;
					char costoTotalStr[20];
					sprintf_s(costoTotalStr, sizeof(costoTotalStr), "%.2f", totalCosto);
					SetDlgItemTextA(hwnd, Edit_Costo_Compra, costoTotalStr);

					char boletosCompradosStr[10];
					sprintf_s(boletosCompradosStr, sizeof(boletosCompradosStr), "%d", boletosComprados);
					SetDlgItemTextA(hwnd, Edit_Cantidad_Compra, boletosCompradosStr);
				}
			}
		}
		else if (LOWORD(wParam) == Boton_Volver_Compra) {
			EndDialog(hwnd, 0);
	DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);		}

		else if (LOWORD(wParam) == Datos) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_Datos), hwnd, VentanaDatos);
		}
		else if (LOWORD(wParam) == Calendario) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
		}
		else if (LOWORD(wParam) == MisCompras) {
			EndDialog(hwnd, 0);
			DialogBox(hInst, MAKEINTRESOURCE(Ventana_MisCompras), hwnd, VentanaMisCompras);

		}
		else if (LOWORD(wParam) == 40004) {
			DestroyWindow(hwnd);
			PostQuitMessage(0);
		}
	} break;

		//CLOSE
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
		DialogBox(hInst, MAKEINTRESOURCE(Ventana_EventosMes), hwnd, VentanaCalendario);
	}break;

	}
	return 0;
}

void inicializarNodos() {
	//Fechas random
	for (int i = 0; i < 36; ++i) {
		fechas[i].dia = obtenerNumeroAleatorio(1, 30);
		fechas[i].mes = obtenerNumeroAleatorio(1, 12);
		fechas[i].anno = 24;
	}
	//Eventos

	//Fechas
	for (int i = 0; i < 36; ++i) {
		eventos[i].fecha = fechas[i];
	}
	//Nombres
	for (int i = 0; i < 36; ++i) {
		snprintf(eventos[i].nombre, sizeof(eventos[i].nombre), "Evento %d", i + 1);
	}

	//cantidadRandom
	for (int i = 0; i < 36; ++i) {
		eventos[i].cantidadInicial = obtenerNumeroAleatorio(1, 100);
		eventos[i].costoInicial = obtenerNumeroAleatorio(1, 300);
		
		eventos[i].costoFinal = eventos[i].costoInicial;
		eventos[i].disponibles = eventos[i].cantidadInicial;
	}

	// Conectar Eventos
	for (int i = 0; i < 35; ++i) {
		eventos[i].siguienteEvento = &eventos[i + 1];
		eventos[i + 1].anteriorEvento = &eventos[i];
	}

	// Inicializar el último evento
	eventos[35].siguienteEvento = nullptr;
	eventos[0].anteriorEvento = nullptr;

	// Asignar cabezaEvento al primer evento
	cabezaEvento = &eventos[0];

	// Inicializar Usuario
	NodoUsuario* nuevoUsuario = new NodoUsuario("Alanis Ayala", "Contraseña123", "Alanis123");

	// Conectar el nuevo usuario al principio de la lista
	nuevoUsuario->siguienteUsuario = cabezaUsuario;
	nuevoUsuario->anteriorUsuario = nullptr;

	if (cabezaUsuario != nullptr) {
		cabezaUsuario->anteriorUsuario = nuevoUsuario;
	}

	cabezaUsuario = nuevoUsuario;

	// Agregar todas las citas al vector temporal
	temporalEvento = cabezaEvento;
	while (temporalEvento != nullptr) {
		vectorTemporal.push_back(temporalEvento);
		if (temporalEvento->siguienteEvento != nullptr) {
			temporalEvento = temporalEvento->siguienteEvento;
		}
		else {
			break;  // Si llegamos al final de la lista, salir del bucle
		}
	}

}

int obtenerNumeroAleatorio(int min, int max) {
	return rand() % (max - min + 1) + min;
}

bool compararPorFecha(const EventoPuntero& evento1, const EventoPuntero& evento2) {
	// Comparar primero por año, luego por mes y finalmente por día
	if (evento1->fecha.anno != evento2->fecha.anno) {
		return evento1->fecha.anno < evento2->fecha.anno;
	}
	else if (evento1->fecha.mes != evento2->fecha.mes) {
		return evento1->fecha.mes < evento2->fecha.mes;
	}
	else {
		return evento1->fecha.dia < evento2->fecha.dia;
	}
}

bool usuarioYaRegistrado(const char* nombre) {
	UsuarioPuntero actual = cabezaUsuario;

	while (actual != nullptr) {
		// Comparar el nombre del usuario actual con el nombre a registrar
		if (strcmp(actual->usuario, nombre) == 0) {
			return true;  // El usuario ya está registrado
		}
		actual = actual->siguienteUsuario;
	}

	return false;  // El usuario no está registrado
}
//LRESULT CALLBACK Prototipo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//	switch (msg)
//	{
//		//INITIAL
//	case WM_INITDIALOG: {
//
//	}break;
//
//		//COMMAND
//	case WM_COMMAND: {
//
//	} break;
//
//		//CLOSE
//	case WM_CLOSE: {
//		DestroyWindow(hwnd);
//		PostQuitMessage(0);
//	}break;
//
//	}
//	return 0;
//}
