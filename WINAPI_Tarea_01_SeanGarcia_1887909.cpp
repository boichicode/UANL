#include <Windows.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <stdlib.h>
#include "Resource.h"
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctime>
#include <Commctrl.h>
#include <CommDlg.h>

using namespace std;
//////////////////Definicion de estructuras//////////////////
struct usuario {
	char username[20];
	char pass[20];
	char nombre_real[25] = { '\0' };
	char alias[20] = { '\0' };
	wchar_t profilepic_path[MAX_PATH];
	int user_id;
	usuario* ant;
	usuario* sig;
};
usuario* usuario_origen;
usuario* usuario_aux;

struct orden {
	char producto[25] = { '\0' };
};

struct producto {
	char name[20];
	int numero_inventario;
	long codigo;
	char marca[20];
	char descrip[100];
	float monto;
	char product_pic_01[MAX_PATH];
	char product_pic_02[MAX_PATH];
	producto* sig;
	producto* ant;
};
producto* producto_origen;
producto* producto_aux;

struct fechaenvio {
	int dd;
	int mm;
	int yyyy;
};
struct envio {
	float total;
	char calle[50];
	char colonia[25];
	char ciudad[25];
	char estado[25];
	char mensaje[100];
	int ID_envio=0;
	wchar_t fechadeenvio[10];
	orden order[25];
	fechaenvio date;
	envio* ant;
	envio* sig;
};
envio* envio_origen;
envio* envio_aux;
envio* envio_fin;

HBITMAP bmp_01;
HBITMAP bmp_02;
HBITMAP bmp_user;
SYSTEMTIME st;
float total_globalbuffer = 0;
int ID_ENVIO_global = 0;
int user_id_global = 0;
int contador_user = 0;
wchar_t pic_buffer[MAX_PATH];
//int contador_users;
//int contador_product;
//int contador_envio;

//////////////////ABC de estructuras//////////////////
void Alta_Producto(producto* nuevo) {
	producto_aux = producto_origen;
	if (producto_origen == NULL) {
		producto_origen = new producto;
		
		strcpy_s(producto_origen->name, nuevo->name);
		producto_origen->numero_inventario= nuevo->numero_inventario;
		producto_origen->codigo = nuevo->codigo;
		strcpy_s(producto_origen->marca, nuevo->marca);
		strcpy_s(producto_origen->descrip, nuevo->descrip);
		producto_origen->monto = nuevo->monto;
		strcpy_s(producto_origen->product_pic_01, nuevo->product_pic_01);
		strcpy_s(producto_origen->product_pic_02, nuevo->product_pic_02);

		producto_origen->sig = NULL;
		producto_origen->ant = NULL;

	}
	else {
		while (producto_aux->sig != NULL)
			producto_aux = producto_aux->sig;

		producto_aux->sig = new producto;
		producto_aux->sig->ant = producto_aux;
		producto_aux = producto_aux->sig;

		strcpy_s(producto_aux->name, nuevo->name);
		producto_aux->numero_inventario = nuevo->numero_inventario;
		producto_aux->codigo = nuevo->codigo;
		strcpy_s(producto_aux->marca, nuevo->marca);
		strcpy_s(producto_aux->descrip, nuevo->descrip);
		producto_aux->monto = nuevo->monto;
		strcpy_s(producto_aux->product_pic_01, nuevo->product_pic_01);
		strcpy_s(producto_aux->product_pic_02, nuevo->product_pic_02);


		producto_aux->sig = NULL;
	}
	producto_aux = producto_origen;
}
void Cambio_Producto(producto* mod) {

		
		producto_aux->numero_inventario = mod->numero_inventario;
		producto_aux->codigo = mod->codigo;
		strcpy_s(producto_aux->marca, mod->marca);
		strcpy_s(producto_aux->descrip, mod->descrip);
		producto_aux->monto = mod->monto;
		strcpy_s(producto_aux->product_pic_01, mod->product_pic_01);
		strcpy_s(producto_aux->product_pic_02, mod->product_pic_02);

}
void Baja_Producto() {
	if (producto_aux->ant == NULL && producto_aux->sig == NULL) {//Unico nodo
		delete producto_aux;
		producto_aux = producto_origen = NULL; //Se borra el unico nodo
	}
	else if (producto_aux->ant == NULL) {//primero
		producto_origen = producto_aux->sig; //Se hace que el siguiente nodo del auxiliar sea el nuevo origen
		delete producto_aux;
		producto_origen->ant = NULL;//Como tenemos otro origen, el anterior a este debe ser nulo
		producto_aux = producto_origen;
	}
	else if (producto_aux->sig == NULL) {//ultimo
		producto_aux->ant->sig = NULL; //Convertimos el siguiente del anterior nodo a nulo 
		delete producto_aux;
		producto_aux = producto_origen;
	}
	else { //entre nodos
		producto_aux->ant->sig = producto_aux->sig; //hace que el siguiente del anterior nodo de nuestro auxiliar sea el siguiente nodo actual del auxiliar
		producto_aux->sig->ant = producto_aux->ant; //hace que el anterior del siguiente nodo de nuestro auxiliar sea el anterior nodo actual del auxiliar
		delete producto_aux;
		producto_aux = producto_origen;
	}
	producto_aux = producto_origen;
}

void Alta_Envio(envio *nuevo) {
	int i = 0;
	envio_aux = envio_origen;
	if (envio_origen == NULL) {
		envio_origen = new envio;

		strcpy_s(envio_origen->calle, nuevo->calle);
		strcpy_s(envio_origen->colonia, nuevo->colonia);
		strcpy_s(envio_origen->ciudad, nuevo->ciudad);
		strcpy_s(envio_origen->estado, nuevo->estado);
		strcpy_s(envio_origen->mensaje, nuevo->mensaje);

		for (i = 0; i < 25; i++)
		{
			strcpy_s(envio_origen->order[i].producto, nuevo->order[i].producto);
		}

		envio_origen->date.dd = nuevo->date.dd;
		envio_origen->date.mm = nuevo->date.mm;
		envio_origen->date.yyyy = nuevo->date.yyyy;
		envio_origen->total = nuevo->total;
		envio_origen->ID_envio = nuevo->ID_envio;

		envio_origen->sig = NULL;
		envio_origen->ant = NULL;

		envio_fin = envio_aux;
	}
	else {
		while (envio_aux->sig != NULL)
			envio_aux = envio_aux->sig;

		envio_aux->sig = new envio;
		envio_aux->sig->ant = envio_aux;
		envio_aux = envio_aux->sig;

		strcpy_s(envio_aux->calle, nuevo->calle);
		strcpy_s(envio_aux->colonia, nuevo->colonia);
		strcpy_s(envio_aux->ciudad, nuevo->ciudad);
		strcpy_s(envio_aux->estado, nuevo->estado);
		strcpy_s(envio_aux->mensaje, nuevo->mensaje);

		for (i = 0; i < 25; i++)
		{
			strcpy_s(envio_aux->order[i].producto, nuevo->order[i].producto);
		}

		envio_aux->date.dd = nuevo->date.dd;
		envio_aux->date.mm = nuevo->date.mm;
		envio_aux->date.yyyy = nuevo->date.yyyy;
		envio_aux->total = nuevo->total;
		envio_aux->ID_envio = nuevo->ID_envio;

		envio_fin = envio_aux;

		envio_aux->sig = NULL;
	}
	ID_ENVIO_global++;
	envio_aux = envio_origen;
	total_globalbuffer = 0;
}
void Cambio_Envio(envio*mod) {

	envio_aux = envio_origen;
	while (envio_aux->sig != NULL) {
		if (envio_aux->date.dd == mod->date.dd && envio_aux->date.mm == mod->date.mm && envio_aux->date.yyyy == mod->date.yyyy)
			break;
		else
			envio_aux = envio_aux->sig;
		if (envio_aux->sig == NULL)
			break;
	}

	strcpy_s(envio_aux->calle, mod->calle);
	strcpy_s(envio_aux->colonia, mod->colonia);
	strcpy_s(envio_aux->ciudad, mod->ciudad);
	strcpy_s(envio_aux->estado, mod->estado);
	strcpy_s(envio_aux->mensaje, mod->mensaje);

}
void Baja_Envio() {
	if (envio_aux->ant == NULL && envio_aux->sig == NULL) {//Unico nodo
		delete envio_aux;
		envio_aux = envio_aux = NULL; //Se borra el unico nodo
	}
	else if (envio_aux->ant == NULL) {//primero
		envio_origen = envio_aux->sig; //Se hace que el siguiente nodo del auxiliar sea el nuevo origen
		delete envio_aux;
		envio_origen->ant = NULL;//Como tenemos otro origen, el anterior a este debe ser nulo
		envio_aux = envio_origen;
	}
	else if (envio_aux->sig == NULL) {//ultimo
		envio_aux->ant->sig = NULL; //Convertimos el siguiente del anterior nodo a nulo 
		delete envio_aux;
		envio_aux = envio_origen;
	}
	else { //entre nodos
		envio_aux->ant->sig = envio_aux->sig; //hace que el siguiente del anterior nodo de nuestro auxiliar sea el siguiente nodo actual del auxiliar
		envio_aux->sig->ant = envio_aux->ant; //hace que el anterior del siguiente nodo de nuestro auxiliar sea el anterior nodo actual del auxiliar
		delete envio_aux;
		envio_aux = envio_origen;
	}
	envio_aux = envio_origen;
}

void Alta_Usuario(usuario* nuevo) {
	usuario_aux = usuario_origen;
	if (usuario_origen == NULL) {
		usuario_origen = new usuario;
		strcpy_s(usuario_origen->username,nuevo->username);
		strcpy_s(usuario_origen->pass, nuevo->pass);
		usuario_origen->user_id = contador_user;

		usuario_origen->sig = NULL;
		usuario_origen->ant = NULL;
	}
	else {
		while (usuario_aux->sig != NULL)
			usuario_aux = usuario_aux->sig;

		usuario_aux->sig = new usuario;
		usuario_aux->sig->ant = usuario_aux;
		usuario_aux = usuario_aux->sig;

		strcpy_s(usuario_aux->username, nuevo->username);
		strcpy_s(usuario_aux->pass, nuevo->pass);
		usuario_aux->user_id = contador_user;

		usuario_aux->sig = NULL;
	}
	usuario_aux = usuario_origen;
	contador_user++;
}
//////////////////Quicksort//////////////////
void swap(envio* a, envio* b)
{
	envio t = *a;
	*a = *b;
	*b = t;

	b->sig = a->sig; b->ant = a->ant;
	a->sig = t.sig; a->ant = t.ant;
}
envio* partition(envio* l, envio* h)
{
	SYSTEMTIME DateSh;
	ZeroMemory(&DateSh, sizeof(DateSh));

	DateSh.wDay = h->date.dd;
	DateSh.wMonth = h->date.mm;
	DateSh.wYear = h->date.yyyy;

	envio* i = l->ant;


	for (envio* j = l; j != h; j = j->sig)
	{
		SYSTEMTIME DateSj;
		ZeroMemory(&DateSj, sizeof(DateSj));
		FILETIME DateFj;
		ZeroMemory(&DateFj, sizeof(DateFj));

		DateSj.wDay = j->date.dd;
		DateSj.wMonth = j->date.mm;
		DateSj.wYear = j->date.yyyy;

		if (DateSj.wYear <= DateSh.wYear && DateSj.wMonth <= DateSh.wMonth && DateSj.wDay < DateSh.wDay)
		{
			if (i == NULL) {
				i = l;
			}
			else {
				i = i->sig;
			}

			swap((i), (j));
		}
	}
	if (i == NULL) {
		i = l;
	}
	else {
		i = i->sig;
	}
	swap((i), (h));
	return i;
}
void quickSort(envio* iniL, envio* finL)
{
	if (finL != NULL && iniL != finL && iniL != finL->sig)
	{
		envio* p = partition(iniL, finL);
		quickSort(iniL, p->ant);
		quickSort(p->sig, finL);
	}

}
//////////////////Escritura y Lectura de Archivos Binarios//////////////////
void escribir_bin(){
	producto_aux = producto_origen;
	envio_aux = envio_origen;
	usuario_aux = usuario_origen;
	
	ofstream writefile;

	writefile.open("C:\\Users\\Latitude E7450\\Documents\\TRABAJOS UANL\\LMAD\\Programación Avanzada\\TERCERAS\\WINAPI_Tarea_01_SeanGarcia_1887909\\WINAPI_Tarea_01_SeanGarcia_1887909\\Productos.bin", ios::out | ios::binary | ios::trunc);
	if (writefile.is_open()) {
		while (producto_aux != nullptr)
		{
			writefile.write((char*)producto_aux, sizeof(producto));
			producto_aux = producto_aux->sig;
		}
		writefile.close();
	}

	writefile.open("C:\\Users\\Latitude E7450\\Documents\\TRABAJOS UANL\\LMAD\\Programación Avanzada\\TERCERAS\\WINAPI_Tarea_01_SeanGarcia_1887909\\WINAPI_Tarea_01_SeanGarcia_1887909\\Envios.bin", ios::out | ios::binary | ios::trunc);
	if (writefile.is_open()) {
		while (envio_aux != nullptr)
		{
			writefile.write((char*)envio_aux, sizeof(envio));
			envio_aux = envio_aux->sig;
		}
		writefile.close();
	}

	writefile.open("C:\\Users\\Latitude E7450\\Documents\\TRABAJOS UANL\\LMAD\\Programación Avanzada\\TERCERAS\\WINAPI_Tarea_01_SeanGarcia_1887909\\WINAPI_Tarea_01_SeanGarcia_1887909\\users.bin", ios::out | ios::binary | ios::trunc);
	if (writefile.is_open()) {
		while (usuario_aux != nullptr)
		{
			writefile.write((char*)usuario_aux, sizeof(usuario));
			usuario_aux = usuario_aux->sig;
		}
		writefile.close();
	}
}
void lectura_bin(){

	ifstream readfile;

	producto_aux = producto_origen;
	envio_aux = envio_origen;
	usuario_aux = usuario_origen;

	readfile.open("C:\\Users\\Latitude E7450\\Documents\\TRABAJOS UANL\\LMAD\\Programación Avanzada\\TERCERAS\\WINAPI_Tarea_01_SeanGarcia_1887909\\WINAPI_Tarea_01_SeanGarcia_1887909\\Productos.bin", ios::in | ios::binary);
	if (readfile.is_open()) {
		producto* readproduct = new producto;

			while (!readfile.read((char*)readproduct, sizeof(producto)).eof()) {

				if (producto_aux == NULL) {
					producto_origen = readproduct;
					producto_origen->sig = NULL;
					producto_origen->ant = NULL;
					producto_aux = producto_origen;

				}
				else {
					while (producto_aux->sig != NULL)
						producto_aux = producto_aux->sig;
					producto_aux->sig = readproduct;
					producto_aux->sig->ant = producto_aux;
					producto_aux = producto_aux->sig;
					producto_aux->sig = NULL;
				}
				readproduct = new producto;
			}
			readfile.close();
	}

	readfile.open("C:\\Users\\Latitude E7450\\Documents\\TRABAJOS UANL\\LMAD\\Programación Avanzada\\TERCERAS\\WINAPI_Tarea_01_SeanGarcia_1887909\\WINAPI_Tarea_01_SeanGarcia_1887909\\Envios.bin", ios::in | ios::binary);
	if (readfile.is_open()) {
		envio* readenvio = new envio;

		while (!readfile.read((char*)readenvio, sizeof(envio)).eof()) {

			if (envio_aux == NULL) {
				envio_origen = readenvio;
				envio_origen->sig = NULL;
				envio_origen->ant = NULL;
				envio_aux = envio_origen;
				envio_fin = envio_aux;

			}
			else {
				while (envio_aux->sig != NULL)
					envio_aux = envio_aux->sig;
				envio_aux->sig = readenvio;
				envio_aux->sig->ant = envio_aux;
				envio_aux = envio_aux->sig;
				envio_aux->sig = NULL;
				envio_fin = envio_aux;
			}
			readenvio = new envio;
		}
		readfile.close();
	}

	readfile.open("C:\\Users\\Latitude E7450\\Documents\\TRABAJOS UANL\\LMAD\\Programación Avanzada\\TERCERAS\\WINAPI_Tarea_01_SeanGarcia_1887909\\WINAPI_Tarea_01_SeanGarcia_1887909\\users.bin", ios::in | ios::binary);
	if (readfile.is_open()) {
		usuario* readuser = new usuario;

		while (!readfile.read((char*)readuser, sizeof(usuario)).eof()) {

			if (usuario_aux == NULL) {
				usuario_origen = readuser;
				usuario_origen->sig = NULL;
				usuario_origen->ant = NULL;
				usuario_aux = usuario_origen;

			}
			else {
				while (usuario_aux->sig != NULL)
					usuario_aux = usuario_aux->sig;
				usuario_aux->sig = readuser;
				usuario_aux->sig->ant = usuario_aux;
				usuario_aux = usuario_aux->sig;
				usuario_aux->sig = NULL;
			}
			readuser = new usuario;
		}
		readfile.close();
		usuario_aux = usuario_origen;
		while (usuario_aux->sig != NULL) {
			contador_user = usuario_aux->user_id;
			usuario_aux = usuario_aux->sig;
			contador_user++;
		}
	}
}

//Instance global
HINSTANCE auxH;

//Handlers de las ventanas principales y menu
HMENU hMyMenu;
HWND IS_WIN;
HWND INFO_WIN;
HWND REG_WIN;

//Handler para las ventanas de producto
HWND producto_nuevo;
HWND producto_mostrar;
HWND producto_editar;
HWND producto_eliminar;

//Handler para las ventanas de envios
HWND envio_nuevo;
HWND envio_mostrar;
HWND envio_editar;
HWND envio_eliminar;

//Callbacks principales
BOOL CALLBACK IS_CALLBACK(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK REG_CALLBACK(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK INFO_CALLBACK(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Callbacks de productos
BOOL CALLBACK nuevoproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK mostrarproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK editarproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK eliminarproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Callbacks de envios
BOOL CALLBACK nuevoenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK mostrarenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK editarenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK eliminarenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int DisplayResourceNAMessageBox(HWND hWnd)
{
	int msgboxID = MessageBox(
		NULL,
		(LPCWSTR)L"Seguro que desea salir?",
		(LPCWSTR)L"System",
		MB_ICONWARNING | MB_YESNO | MB_DEFBUTTON2
	);
	switch (msgboxID)
	{
	case IDYES:
		DestroyWindow(hWnd);
		ShowWindow(IS_WIN, SW_SHOW);
		return FALSE;
	case IDNO:
		return FALSE;
	}

	return msgboxID;
}
void Selecciondemenu(HWND hWnd, UINT msg, WPARAM wParam) {
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case INFO_MENU:
			ShowWindow(hWnd, SW_HIDE);
			INFO_WIN = CreateDialog(auxH, MAKEINTRESOURCE(INFO_WIN_MAIN), NULL, INFO_CALLBACK);
			SetMenu(INFO_WIN, hMyMenu);
			ShowWindow(INFO_WIN, SW_SHOW);
			break;
		case ID_PRODUCTOS_EDITARPRODUCTO:
			ShowWindow(hWnd, SW_HIDE);
			producto_editar = CreateDialog(auxH, MAKEINTRESOURCE(EDITP_WIN), NULL, editarproducto_CB);
			SetMenu(producto_editar, hMyMenu);
			ShowWindow(producto_editar, SW_SHOW);
			break;
		case ID_PRODUCTOS_ELIMINARPRODUCTO:
			ShowWindow(hWnd, SW_HIDE);
			producto_eliminar = CreateDialog(auxH, MAKEINTRESOURCE(DELETEP_WIN), NULL, eliminarproducto_CB);
			SetMenu(producto_eliminar, hMyMenu);
			ShowWindow(producto_eliminar, SW_SHOW);
			break;
		case ID_PRODUCTOS_MISPRODUCTOS:
			ShowWindow(hWnd, SW_HIDE);
			producto_mostrar = CreateDialog(auxH, MAKEINTRESOURCE(SHOWP_WIN), NULL, mostrarproducto_CB);
			SetMenu(producto_mostrar, hMyMenu);
			ShowWindow(producto_mostrar, SW_SHOW);
			break;
		case ID_PRODUCTOS_NUEVOPRODUCTO:
			ShowWindow(hWnd, SW_HIDE);
			producto_nuevo = CreateDialog(auxH, MAKEINTRESOURCE(NEWP_WIN), NULL, nuevoproducto_CB);
			SetMenu(producto_nuevo, hMyMenu);
			ShowWindow(producto_nuevo, SW_SHOW);
			break;
		case ID_ENV32778://Mis envios
			ShowWindow(hWnd, SW_HIDE);
			envio_mostrar = CreateDialog(auxH, MAKEINTRESOURCE(SHOWSP_WIN), NULL, mostrarenvio_CB);
			SetMenu(envio_mostrar, hMyMenu);
			ShowWindow(envio_mostrar, SW_SHOW);
			break;
		case ID_ENV32779://Comprar Productos
			ShowWindow(hWnd, SW_HIDE);
			envio_nuevo = CreateDialog(auxH, MAKEINTRESOURCE(NEWSP_WIN), NULL, nuevoenvio_CB);
			SetMenu(envio_nuevo, hMyMenu);
			ShowWindow(envio_nuevo, SW_SHOW);
			break;
		case ID_ENV32780://Editar Envios
			ShowWindow(hWnd, SW_HIDE);
			envio_editar = CreateDialog(auxH, MAKEINTRESOURCE(EDITSP_WIN), NULL, editarenvio_CB);
			SetMenu(envio_editar, hMyMenu);
			ShowWindow(envio_editar, SW_SHOW);
			break;
		case ID_ENV32781://Cancelar Envios
			ShowWindow(hWnd, SW_HIDE);
			envio_eliminar = CreateDialog(auxH, MAKEINTRESOURCE(DELETESP_WIN), NULL, eliminarenvio_CB);
			SetMenu(envio_eliminar, hMyMenu);
			ShowWindow(envio_eliminar, SW_SHOW);
			break;
		case MENU_EXIT: //exit de menu
			DisplayResourceNAMessageBox(hWnd);
			hMyMenu = LoadMenu(auxH, MAKEINTRESOURCE(PA_MENU));
			break;
		/*case ID_OPCIONES_PRUEBABMP:
			ShowWindow(hWnd, SW_HIDE);
			pictest = CreateDialog(auxH, MAKEINTRESOURCE(PruebaBmp), NULL, bmp_CB);
			SetMenu(pictest, hMyMenu);
			ShowWindow(pictest, SW_SHOW);
			break;*/
		}
		break;
	}
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, PSTR cmdLine, int showCmd) {
	lectura_bin();
	auxH = hInst;
	IS_WIN = CreateDialog(auxH, MAKEINTRESOURCE(IS_WIN_MAIN), NULL, IS_CALLBACK);
	hMyMenu = LoadMenu(hInst, MAKEINTRESOURCE(PA_MENU));
	ShowWindow(IS_WIN, SW_SHOW);

	MSG msg;

	while (GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}

BOOL CALLBACK IS_CALLBACK(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HWND EDIT_USUARIO = GetDlgItem(hWnd, LOGIN_EDIT_USER);
	HWND EDIT_PASS = GetDlgItem(hWnd, LOGIN_EDIT_PASS);
	char buffer[20];
	bool ctrl_user = false;
	bool ctrl_pass = false;
	int i, len;
	usuario_aux = usuario_origen;
	switch (msg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case LOGIN_ING:
			if (usuario_origen == NULL) {
				::MessageBox(NULL, L"No hay usuarios registrados", L"Aviso", MB_OK || MB_ICONEXCLAMATION);
				break;
			}
			SendMessageA(EDIT_USUARIO, WM_GETTEXT, sizeof(buffer) / sizeof(buffer[0]), reinterpret_cast<LPARAM>(buffer));
			len = SendMessageA(EDIT_USUARIO, WM_GETTEXTLENGTH, 0, 0);
			while (ctrl_user != true) {
				for (i = 0; i < len; i++) {
					ctrl_user = buffer[i] == usuario_aux->username[i];
					if (ctrl_user == false)
						break;
				}
				if (usuario_origen->sig == NULL)
					break;
				if (usuario_aux->sig == NULL)
					break;
				if (ctrl_user == false)
					usuario_aux = usuario_aux->sig;
			}
			SendMessageA(EDIT_PASS, WM_GETTEXT, sizeof(buffer) / sizeof(buffer[0]), reinterpret_cast<LPARAM>(buffer));
			len = SendMessageA(EDIT_PASS, WM_GETTEXTLENGTH, 0, 0);
			for (i = 0; i < len; i++) {
				ctrl_pass = buffer[i] == usuario_aux->pass[i];
				if (ctrl_pass == false)
					break;
			}
			if (ctrl_user == true && ctrl_pass == true) {
				user_id_global =usuario_aux->user_id;
				ShowWindow(hWnd, SW_HIDE);
				INFO_WIN = CreateDialog(auxH, MAKEINTRESOURCE(INFO_WIN_MAIN), NULL, INFO_CALLBACK);
				SetMenu(INFO_WIN, hMyMenu);
				ShowWindow(INFO_WIN, SW_SHOW);
			}
			else
				::MessageBox(NULL, L"Usuario y/o Contraseña Incorrectos", L"Aviso", MB_OK || MB_ICONERROR);
			break;
		case LOGIN_REG:
			ShowWindow(hWnd, SW_HIDE);
			REG_WIN = CreateDialog(auxH, MAKEINTRESOURCE(REG_WIN_MAIN), NULL, REG_CALLBACK);
			ShowWindow(REG_WIN, SW_SHOW);
			break;
		case LOGIN_EXIT:
			DisplayResourceNAMessageBox(hWnd);
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}
	return FALSE;
}

BOOL CALLBACK INFO_CALLBACK(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	HBITMAP profile_pic;
	usuario* infouser = new usuario;
	switch (msg)
	{
	case WM_INITDIALOG:
		usuario_aux = usuario_origen;
		while (usuario_aux->sig != NULL) {
			if (usuario_aux->user_id == user_id_global)
				break;
			else
				usuario_aux = usuario_aux->sig;
		}
		profile_pic = (HBITMAP)LoadImage(NULL,
			usuario_aux->profilepic_path,
			IMAGE_BITMAP,
			100,
			100,
			LR_LOADFROMFILE
		);
		SendDlgItemMessage(hWnd, INFO_BITMAP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)profile_pic);
		SetDlgItemTextA(hWnd, INFO_STATIC_USER, usuario_aux->username);
		SetDlgItemTextA(hWnd, INFO_EDIT_NAME, usuario_aux->nombre_real);
		SetDlgItemTextA(hWnd, INFO_EDIT_ALIAS, usuario_aux->alias);

		break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case INFO_BTT_EDIT:
			usuario_aux = usuario_origen;
			while (usuario_aux->sig != NULL) {
				if (usuario_aux->user_id == user_id_global)
					break;
				else
					usuario_aux = usuario_aux->sig;
			}
			GetDlgItemTextA(hWnd, INFO_EDIT_NAME, infouser->nombre_real, sizeof(infouser->nombre_real));
			GetDlgItemTextA(hWnd, INFO_EDIT_ALIAS, infouser->alias, sizeof(infouser->alias));

			strcpy_s(usuario_aux->nombre_real,infouser->nombre_real);
			strcpy_s(usuario_aux->alias, infouser->alias);

			SetDlgItemTextA(hWnd, INFO_EDIT_NAME, usuario_aux->nombre_real);
			SetDlgItemTextA(hWnd, INFO_EDIT_ALIAS, usuario_aux->alias);
			escribir_bin();

			break;
		case INFO_BTT_PIC:
		{
			OPENFILENAME ofn;
			wchar_t filename[MAX_PATH];
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = filename;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(filename);
			ofn.lpstrFilter = L"BMP Files(*.BMP)\0*.BMP;\0";
			if (GetOpenFileName(&ofn)) {
				wcscpy_s(infouser->profilepic_path, filename);
				wcscpy_s(usuario_aux->profilepic_path, infouser->profilepic_path);
				profile_pic = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE);
				SendDlgItemMessage(hWnd, INFO_BITMAP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)profile_pic);
			}
			else
				::MessageBox(NULL, L"No se logro completar la acción.", L"Error", MB_OK);
		}
		}
	}
	break;
	}
	delete infouser;
	return FALSE;
}

BOOL CALLBACK REG_CALLBACK(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool aux = false;

	switch (msg) {
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case REG_BTT_REG: {
			usuario* nuevouser = new usuario;
			GetDlgItemTextA(hWnd, LOGIN_EDIT_USER, nuevouser->username, sizeof(nuevouser->username));
			GetDlgItemTextA(hWnd, LOGIN_EDIT_PASS, nuevouser->pass, sizeof(nuevouser->pass));

			usuario_aux = usuario_origen;
			if (usuario_origen == NULL) {
				Alta_Usuario(nuevouser);
				escribir_bin();

				delete nuevouser;
				DestroyWindow(hWnd);
				ShowWindow(IS_WIN, SW_SHOW);
			}
			else {
				while (aux != true) {
					int len = SendMessageA(GetDlgItem(hWnd, LOGIN_EDIT_USER), WM_GETTEXTLENGTH, 0, 0);
					for (int i = 0; i < len; i++) {
						aux = nuevouser->username[i] == usuario_aux->username[i];
						if (aux == false)
							break;
					}
					if (usuario_origen->sig == NULL)
						break;
					if (usuario_aux->sig == NULL)
						break;
					if (aux == false)
						usuario_aux = usuario_aux->sig;
				}
				if (aux != true) {
					Alta_Usuario(nuevouser);
					escribir_bin();

					delete nuevouser;
					DestroyWindow(hWnd);
					ShowWindow(IS_WIN, SW_SHOW);
				}
				else {
					::MessageBox(NULL, L"Nombre de usuario existente", L"Aviso", MB_OK || MB_ICONERROR);
					delete nuevouser;
				}
			}
		}
			break;
		case REG_BTT_EXIT:
			DestroyWindow(hWnd);
			ShowWindow(IS_WIN, SW_SHOW);
			break;
		}
	break;
	}
	return FALSE;
}
////////Definicion de callbacks de producto////////
BOOL CALLBACK nuevoproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	producto* agregar = new producto;
	char numero[15];
	bool codecheck=false;
	switch (msg)
	{
	case WM_COMMAND: 
		switch (LOWORD(wParam))
		{
		case NEWP_BTT_ADD:

			GetDlgItemTextA(hWnd, NEWP_EDIT_NAME, agregar->name, sizeof(agregar->name));
			GetDlgItemTextA(hWnd, NEWP_EDIT_MARCA, agregar->marca, sizeof(agregar->marca));
			GetDlgItemTextA(hWnd, NEWP_EDIT_DESC, agregar->descrip, sizeof(agregar->descrip));
			GetDlgItemTextA(hWnd, NEWP_EDIT_QUANT, numero, sizeof(numero));
			agregar->numero_inventario = atoi(numero);
			GetDlgItemTextA(hWnd, NEWP_EDIT_CODE, numero, sizeof(numero));
			agregar->codigo = atol(numero);
			GetDlgItemTextA(hWnd, NEWP_EDIT_PRECIO, numero, sizeof(numero));
			agregar->monto = atof(numero);
			GetDlgItemTextA(hWnd,NEWP_PICSTATIC, agregar->product_pic_01,MAX_PATH);
			GetDlgItemTextA(hWnd, NEWP_PICSTATIC2, agregar->product_pic_02, MAX_PATH);
			if (producto_origen == NULL) {
				Alta_Producto(agregar);
				delete agregar;
				escribir_bin();
				::MessageBox(NULL, L"Producto Agregado", L"System", MB_OK);
				SetDlgItemTextA(hWnd, NEWP_EDIT_NAME, "");
				SetDlgItemTextA(hWnd, NEWP_EDIT_MARCA, "");
				SetDlgItemTextA(hWnd, NEWP_EDIT_DESC, "");
				SetDlgItemTextA(hWnd, NEWP_EDIT_QUANT, "");
				SetDlgItemTextA(hWnd, NEWP_EDIT_CODE, "");
				SetDlgItemTextA(hWnd, NEWP_EDIT_PRECIO, "");
				SetDlgItemTextA(hWnd, NEWP_PICSTATIC, "");
				SetDlgItemTextA(hWnd, NEWP_PICSTATIC2, "");
			}
			else {
				producto_aux = producto_origen;
				while (producto_aux->sig != NULL) {
					codecheck = agregar->codigo == producto_aux->codigo;
					if (codecheck == true)
						break;
					if (producto_aux->sig == NULL)
						break;
					producto_aux = producto_aux->sig;
				}
				if (codecheck == false) {
					Alta_Producto(agregar);
					delete agregar;
					escribir_bin();
					::MessageBox(NULL, L"Producto Agregado", L"System", MB_OK);
					SetDlgItemTextA(hWnd, NEWP_EDIT_NAME, "");
					SetDlgItemTextA(hWnd, NEWP_EDIT_MARCA, "");
					SetDlgItemTextA(hWnd, NEWP_EDIT_DESC, "");
					SetDlgItemTextA(hWnd, NEWP_EDIT_QUANT, "");
					SetDlgItemTextA(hWnd, NEWP_EDIT_CODE, "");
					SetDlgItemTextA(hWnd, NEWP_EDIT_PRECIO, "");
					SetDlgItemTextA(hWnd, NEWP_PICSTATIC, "");
					SetDlgItemTextA(hWnd, NEWP_PICSTATIC2, "");
				}
				else
					::MessageBox(NULL, L"Ya existe un producto con el codigo introducido", L"System", MB_OK);
			}
			break;

		case NEWP_BTT_PIC: {
			OPENFILENAME ofn;
			wchar_t filename[MAX_PATH];
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = filename;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = 100;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.lpstrFilter = L"BMP Files(*.BMP)\0*.BMP;\0";
			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hWnd, NEWP_PICSTATIC, filename);
				bmp_01 = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hWnd, NEWP_PICCTRL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp_01);
			}
			else
				::MessageBox(NULL, L"No se logro completar la acción.", L"Error", MB_OK);
		}
			break;
		case NEWP_BTT_PIC2: {
			OPENFILENAME ofn;
			wchar_t filename[MAX_PATH];
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = filename;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = 100;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.lpstrFilter = L"BMP Files(*.BMP)\0*.BMP;\0";
			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hWnd, NEWP_PICSTATIC2, filename);
				bmp_02 = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hWnd, NEWP_PICCTRL2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp_02);
			}
			else
				::MessageBox(NULL, L"No se logro completar la acción.", L"Error", MB_OK);
		}
			break;
		}
	break;
	}
	return FALSE;
}
BOOL CALLBACK mostrarproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	int i=0,selec,len;
	char buffer[20]{};
	bool ctrl = false;
	HWND CB_PROD = GetDlgItem(hWnd, SHOWP_CB_PROD);
	switch (msg)
	{
	case WM_INITDIALOG:
		producto_aux = producto_origen;
		if (producto_origen == NULL)
			::MessageBox(NULL, L"No hay productos agregados", L"Error", MB_OK || MB_ICONEXCLAMATION);
		else
		{
			do {
				SendMessageA(CB_PROD, CB_INSERTSTRING, i, (LPARAM)producto_aux->name);
				i++;
				if (producto_origen->sig == NULL)
					break;
				producto_aux = producto_aux->sig;
			} while (producto_aux->sig != NULL);
			SendMessageA(CB_PROD, CB_INSERTSTRING, i, (LPARAM)producto_aux->name);
			i++;
			producto_aux = producto_aux->sig;
		}
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case SHOWP_BTT_SHOW:
			selec = SendMessageA(CB_PROD, CB_GETCURSEL, 0, 0);
			len = SendMessageA(CB_PROD, CB_GETLBTEXTLEN, selec, 0);
			SendMessageA(CB_PROD, CB_GETLBTEXT, selec, reinterpret_cast<LPARAM>(buffer));
			producto_aux = producto_origen;
			while (ctrl != true) {
				for (i = 0; i < len; i++) {
					ctrl = producto_aux->name[i] == buffer[i];
					if (ctrl == false)
						break;
				}
				if (ctrl == false)
					producto_aux = producto_aux->sig;
				if (producto_aux->sig == NULL)
					break;
			}
			SetDlgItemTextA(hWnd, SHOWP_EDIT_NAME, producto_aux->name);
			SetDlgItemTextA(hWnd, SHOWP_EDIT_MARCA, producto_aux->marca);
			_ltoa_s(producto_aux->codigo, buffer, 10);
			SetDlgItemTextA(hWnd, SHOWP_EDIT_CODE, buffer);
			SetDlgItemTextA(hWnd, SHOWP_EDIT_PRECIO, to_string(producto_aux->monto).c_str());
			SetDlgItemTextA(hWnd, SHOWP_EDIT_DESC, producto_aux->descrip);
			_itoa_s(producto_aux->numero_inventario, buffer, 10);
			SetDlgItemTextA(hWnd, SHOWP_EDIT_QUANT,buffer);
			bmp_01 = (HBITMAP)LoadImage(NULL, reinterpret_cast<LPCWSTR>(producto_aux->product_pic_01), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			bmp_02 = (HBITMAP)LoadImage(NULL, reinterpret_cast<LPCWSTR>(producto_aux->product_pic_02), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			return FALSE;
		}
		break;
	}
	return FALSE;
}
BOOL CALLBACK editarproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	HWND CB_PROD=GetDlgItem(hWnd, EDITP_CB_PROD);
	int i = 0,selec=0,len=0;
	char buffer[20]{};
	bool ctrl = false;
	producto* agregar = new producto;
	char numero[15];
	switch (msg)
	{
	case WM_INITDIALOG:
		producto_aux = producto_origen;
		if (producto_origen == NULL)
			::MessageBox(NULL, L"No hay productos agregados", L"Error", MB_OK || MB_ICONEXCLAMATION);
		else
		{
			do {
				SendMessageA(CB_PROD, CB_INSERTSTRING, i, (LPARAM)producto_aux->name);
				i++;
				if (producto_origen->sig == NULL)
					break;
				producto_aux = producto_aux->sig;
			} while (producto_aux->sig != NULL);
			SendMessageA(CB_PROD, CB_INSERTSTRING, i, (LPARAM)producto_aux->name);
			i++;
			producto_aux = producto_aux->sig;
		}
		return FALSE;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case EDITP_BTT_SHOW:
			selec = SendMessageA(CB_PROD, CB_GETCURSEL, 0, 0);
			len = SendMessageA(CB_PROD, CB_GETLBTEXTLEN, selec, 0);
			SendMessageA(CB_PROD, CB_GETLBTEXT, selec, reinterpret_cast<LPARAM>(buffer));
			producto_aux = producto_origen;
			while (ctrl != true) {
				for (i = 0; i < len; i++) {
					ctrl = producto_aux->name[i] == buffer[i];
					if (ctrl == false)
						break;
				}
				if (ctrl == false)
					producto_aux = producto_aux->sig;
				if (producto_aux->sig == NULL)
					break;
			}
			SetDlgItemTextA(hWnd, EDITP_EDIT_NAME, producto_aux->name);
			SetDlgItemTextA(hWnd, EDITP_EDIT_MARCA, producto_aux->marca);
			_ltoa_s(producto_aux->codigo, buffer, 10);
			SetDlgItemTextA(hWnd,EDITP_EDIT_CODE, buffer);
			SetDlgItemTextA(hWnd, EDITP_EDIT_PRECIO, to_string(producto_aux->monto).c_str());
			SetDlgItemTextA(hWnd, EDITP_EDIT_DESC, producto_aux->descrip);
			_itoa_s(producto_aux->numero_inventario, buffer, 10);
			SetDlgItemTextA(hWnd, EDITP_EDIT_QUANT, buffer);
			bmp_01 = (HBITMAP)LoadImage(NULL, reinterpret_cast<LPCWSTR>(producto_aux->product_pic_01), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			bmp_02 = (HBITMAP)LoadImage(NULL, reinterpret_cast<LPCWSTR>(producto_aux->product_pic_02), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			break;
		case EDITP_BTT_ADD:

			GetDlgItemTextA(hWnd, EDITP_EDIT_NAME, agregar->name, sizeof(agregar->name));
			GetDlgItemTextA(hWnd, EDITP_EDIT_MARCA, agregar->marca, sizeof(agregar->marca));
			GetDlgItemTextA(hWnd, EDITP_EDIT_DESC, agregar->descrip, sizeof(agregar->descrip));
			GetDlgItemTextA(hWnd, EDITP_EDIT_QUANT, numero, sizeof(numero));
			agregar->numero_inventario = atoi(numero);
			GetDlgItemTextA(hWnd, EDITP_EDIT_CODE, numero, sizeof(numero));
			agregar->codigo = atol(numero);
			GetDlgItemTextA(hWnd, EDITP_EDIT_PRECIO, numero, sizeof(numero));
			agregar->monto = atof(numero);
			GetDlgItemTextA(hWnd, EDITP_PICSTATIC, agregar->product_pic_01, MAX_PATH);
			GetDlgItemTextA(hWnd, EDITP_PICSTATIC2, agregar->product_pic_02, MAX_PATH);
			Cambio_Producto(agregar);
			delete agregar;

			escribir_bin();

			::MessageBox(NULL, L"Producto Modificado", L"System", MB_OK);

			SetDlgItemTextA(hWnd, EDITP_EDIT_NAME, "");
			SetDlgItemTextA(hWnd, EDITP_EDIT_MARCA, "");
			SetDlgItemTextA(hWnd, EDITP_EDIT_DESC, "");
			SetDlgItemTextA(hWnd, EDITP_EDIT_QUANT, "");
			SetDlgItemTextA(hWnd, EDITP_EDIT_CODE, "");
			SetDlgItemTextA(hWnd, EDITP_EDIT_PRECIO, "");
			SetDlgItemTextA(hWnd, EDITP_PICSTATIC, "");
			SetDlgItemTextA(hWnd, EDITP_PICSTATIC2, "");
			break;

		case EDITP_BTT_PIC:{
			OPENFILENAME ofn;
			wchar_t filename[MAX_PATH];
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = filename;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = 100;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.lpstrFilter = L"BMP Files(*.BMP)\0*.BMP;\0";
			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hWnd, EDITP_PICSTATIC, filename);
				bmp_01 = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hWnd, NEWP_PICCTRL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp_01);
			}
			else
				::MessageBox(NULL, L"No se logro completar la acción.", L"Error", MB_OK);
		}
			break;
		case EDITP_BTT_PIC2: {
			OPENFILENAME ofn;
			wchar_t filename[MAX_PATH];
			ZeroMemory(&ofn, sizeof(OPENFILENAME));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = filename;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = 100;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.lpstrFilter = L"BMP Files(*.BMP)\0*.BMP;\0";
			if (GetOpenFileName(&ofn)) {
				SetDlgItemText(hWnd, EDITP_PICSTATIC, filename);
				bmp_02 = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
				SendDlgItemMessage(hWnd, NEWP_PICCTRL, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp_02);
			}
			else
				::MessageBox(NULL, L"No se logro completar la acción.", L"Error", MB_OK);
		}
			break;
		}
	}
		break;
	}
	return FALSE;
}
BOOL CALLBACK eliminarproducto_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	HWND CB_PROD = GetDlgItem(hWnd, DLTP_CB_PROD);
	int i = 0, selec, len;
	char buffer[20]{};
	bool ctrl = false;
	switch (msg)
	{
	case WM_INITDIALOG:
		producto_aux = producto_origen;
		if (producto_origen == NULL)
			::MessageBox(NULL, L"No hay productos agregados", L"Error", MB_OK || MB_ICONEXCLAMATION);
		else
		{
			do {
				SendMessageA(CB_PROD, CB_INSERTSTRING, i, (LPARAM)producto_aux->name);
				i++;
				if (producto_origen->sig == NULL)
					break;
				producto_aux = producto_aux->sig;
			} while (producto_aux->sig != NULL);
			SendMessageA(CB_PROD, CB_INSERTSTRING, i, (LPARAM)producto_aux->name);
			i++;
			producto_aux = producto_aux->sig;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)){
		case DLTP_BTT_SHOW:
			selec = SendMessageA(CB_PROD, CB_GETCURSEL, 0, 0);
			len = SendMessageA(CB_PROD, CB_GETLBTEXTLEN, selec, 0);
			SendMessageA(CB_PROD, CB_GETLBTEXT, selec, reinterpret_cast<LPARAM>(buffer));
			producto_aux = producto_origen;
			while (ctrl != true) {
				for (i = 0; i < len; i++) {
					ctrl = producto_aux->name[i] == buffer[i];
					if (ctrl == false)
						break;
				}
				if (ctrl == false)
					producto_aux = producto_aux->sig;
				if (producto_aux->sig == NULL)
					break;
			}
			SetDlgItemTextA(hWnd, DLTP_EDIT_NAME, producto_aux->name);
			SetDlgItemTextA(hWnd, DLTP_EDIT_MARCA, producto_aux->marca);
			_ltoa_s(producto_aux->codigo, buffer, 10);
			SetDlgItemTextA(hWnd, DLTP_EDIT_CODE, buffer);
			SetDlgItemTextA(hWnd, DLTP_EDIT_PRECIO, to_string(producto_aux->monto).c_str());
			SetDlgItemTextA(hWnd, DLTP_EDIT_DESC, producto_aux->descrip);
			_itoa_s(producto_aux->numero_inventario, buffer, 10);
			SetDlgItemTextA(hWnd, DLTP_EDIT_QUANT, buffer);
			bmp_01 = (HBITMAP)LoadImage(NULL, reinterpret_cast<LPCWSTR>(producto_aux->product_pic_01), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			bmp_02 = (HBITMAP)LoadImage(NULL, reinterpret_cast<LPCWSTR>(producto_aux->product_pic_02), IMAGE_BITMAP, 100, 100, LR_LOADFROMFILE | LR_DEFAULTSIZE);
			break;
		case DLTP_BTT_DLTP:
			Baja_Producto();
			escribir_bin();
			break;
		}
		break;
	}
	return FALSE;
}
////////Definicion de callbacks de envio////////
BOOL CALLBACK nuevoenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	HWND CB_PRODUCT = GetDlgItem(hWnd, NEWSP_CB_PRODSELEC);
	HWND CB_CANTIDAD = GetDlgItem(hWnd, NEWSP_CB_CANTIDAD);
	HWND LB_ORDEN = GetDlgItem(hWnd, NEWSP_LB_ORDEN);
	HWND LB_TOTAL = GetDlgItem(hWnd, NEWSP_LB_TOTAL);
	char buffer_name[20], buffer_cantidad[20];
	int i = 0, j = 0;
	int selec, len, quantity;
	bool ctrl = false;
	float total = 0, total_buffer;
	envio* agregar = new envio;
	envio_aux = envio_origen;
	producto_aux = producto_origen;
	switch (msg) {
	case WM_INITDIALOG:
		if (producto_origen == NULL)
			::MessageBox(NULL, L"No hay productos agregados", L"Error", MB_OK || MB_ICONEXCLAMATION);
		else
		{
			do {
				SendMessageA(CB_PRODUCT, CB_ADDSTRING, i, (LPARAM)producto_aux->name);
				i++;

				if (producto_origen->sig == NULL)
					break;

				producto_aux = producto_aux->sig;
			} while (producto_aux->sig != NULL);
			SendMessageA(CB_PRODUCT, CB_ADDSTRING, i, (LPARAM)producto_aux->name);
			i++;
			producto_aux = producto_aux->sig;
		}
		ZeroMemory(&st, sizeof(st));
		st.wDay = agregar->date.dd;
		st.wMonth = agregar->date.mm;
		st.wYear = agregar->date.yyyy;
		SendDlgItemMessage(hWnd, NEWSP_DATEPKR, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&st);
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case NEWSP_BTT_PRODSELEC: {
			selec = SendMessageA(CB_PRODUCT, CB_GETCURSEL, 0, 0);
			len = SendMessageA(CB_PRODUCT, CB_GETLBTEXTLEN, selec, 0);
			SendMessageA(CB_PRODUCT, CB_GETLBTEXT, selec, reinterpret_cast<LPARAM>(buffer_name));
			producto_aux = producto_origen;
			while (ctrl != true) {
				for (i = 0; i < len; i++) {
					ctrl = producto_aux->name[i] == buffer_name[i];
					if (ctrl == false)
						break;
				}
				if (ctrl == false)
					producto_aux = producto_aux->sig;
				if (producto_aux->sig == NULL)
					break;
			}
			SendMessageA(CB_CANTIDAD, CB_RESETCONTENT, 0, 0);
			for (i = 0; i < producto_aux->numero_inventario + 1; i++)
			{
				/*char* aux2 = new char;
				strcpy_s(aux2, sizeof(aux2), to_string(i).c_str());*/
				SendMessageA(CB_CANTIDAD, CB_INSERTSTRING, i, (LPARAM)to_string(i).c_str());
			}
		}
		 break;
		case NEWSP_BTT_ADDP: {
			ctrl = false;
			quantity = SendMessageA(CB_CANTIDAD, CB_GETCURSEL, 0, 0);
			SendMessageA(CB_CANTIDAD, CB_GETLBTEXT, quantity, reinterpret_cast<LPARAM>(buffer_cantidad));
			j = atoi(buffer_cantidad);
			selec = SendMessageA(CB_PRODUCT, CB_GETCURSEL, 0, 0);
			SendMessageA(CB_PRODUCT, CB_GETLBTEXT, selec, reinterpret_cast<LPARAM>(buffer_name));
			for (i = 0; i < j; i++)
				SendMessageA(LB_ORDEN, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(buffer_name));
			len = SendMessageA(CB_PRODUCT, CB_GETLBTEXTLEN, selec, 0);
			producto_aux = producto_origen;
			while (ctrl != true) {
				for (i = 0; i < len; i++) {
					ctrl = producto_aux->name[i] == buffer_name[i];
					if (ctrl == false)
						break;
				}
				if (ctrl == false)
					producto_aux = producto_aux->sig;
				if (producto_aux->sig == NULL)
					break;
			}
			total_buffer = j * producto_aux->monto;
			total_globalbuffer = total_globalbuffer + total_buffer;

			SendMessageA(LB_TOTAL, LB_RESETCONTENT, 0, 0);
			SendMessageA(LB_TOTAL, LB_INSERTSTRING, 0, reinterpret_cast<LPARAM>(to_string(total_globalbuffer).c_str()));

			producto_aux->numero_inventario = producto_aux->numero_inventario - j;
			SendMessageA(CB_CANTIDAD, CB_RESETCONTENT, 0, 0);
		}
		break;
		case NEWSP_BTT_FIN:
			GetDlgItemTextA(hWnd, NEWSP_EDIT_CALLE, agregar->calle, sizeof(agregar->calle));
			GetDlgItemTextA(hWnd, NEWSP_EDIT_COL, agregar->colonia, sizeof(agregar->colonia));
			GetDlgItemTextA(hWnd, NEWSP_EDIT_CIUDAD, agregar->ciudad, sizeof(agregar->ciudad));
			GetDlgItemTextA(hWnd, NEWSP_EDIT_ESTADO, agregar->estado, sizeof(agregar->estado));
			GetDlgItemTextA(hWnd, NEWSP_EDIT_MSG, agregar->mensaje, sizeof(agregar->mensaje));

			SendDlgItemMessage(hWnd, NEWSP_DATEPKR, DTM_GETSYSTEMTIME, 0, (LPARAM)&st);
			agregar->date.dd = st.wDay;
			agregar->date.mm = st.wMonth;
			agregar->date.yyyy = st.wYear;

			j = 0;
			j = SendMessageA(GetDlgItem(hWnd, NEWSP_LB_ORDEN), LB_GETCOUNT, 0, 0);
			for (i = 0; i < j; i++) {
				SendMessageA(GetDlgItem(hWnd, NEWSP_LB_ORDEN), LB_GETTEXT, i, reinterpret_cast<LPARAM>(agregar->order[i].producto));
			}

			/*GetDlgItemText(hWnd, NEWSP_DATEPKR,agregar->fechadeenvio, sizeof(agregar->fechadeenvio));*/

			agregar->ID_envio = ID_ENVIO_global;
			agregar->total = total_globalbuffer;

			Alta_Envio(agregar);
			delete agregar;

			escribir_bin();

			::MessageBox(NULL, L"Envio Agregado", L"System", MB_OK);

			SetDlgItemTextA(hWnd, NEWSP_EDIT_CALLE, "");
			SetDlgItemTextA(hWnd, NEWSP_EDIT_COL, "");
			SetDlgItemTextA(hWnd, NEWSP_EDIT_CIUDAD, "");
			SetDlgItemTextA(hWnd, NEWSP_EDIT_ESTADO, "");
			SetDlgItemTextA(hWnd, NEWSP_EDIT_MSG, "");
			SendMessageA(LB_ORDEN, LB_RESETCONTENT, 0, 0);
			SendMessageA(LB_TOTAL, LB_RESETCONTENT, 0, 0);

			break;
		}
		break;
	}

	return FALSE;
}
BOOL CALLBACK mostrarenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	int i = 0, j = 0;
	int dd = 0, mm = 0, yyyy = 0;
	char fechas[12];
	SYSTEMTIME fechaHoy;
	ZeroMemory(&fechaHoy, sizeof(fechaHoy));
	GetSystemTime(&fechaHoy);
	envio_aux = envio_origen;
	switch (msg)
	{
	case WM_INITDIALOG:
		if (envio_origen == NULL)
			::MessageBox(NULL, L"No hay ordenes agregadas", L"Error", MB_OK || MB_ICONEXCLAMATION);
		else
		{
			quickSort(envio_origen, envio_fin);
			envio_aux = envio_origen;
			while (envio_aux != nullptr) {
				dd = envio_aux->date.dd;
				mm = envio_aux->date.mm;
				yyyy = envio_aux->date.yyyy;
						if (dd < 10){
							strcpy_s(fechas, "0");
							strcat_s(fechas, to_string(dd).c_str());
						}
						else
							strcpy_s(fechas, to_string(dd).c_str());
						strcat_s(fechas, "/");
						if (mm < 10)
							strcat_s(fechas, "0");
						strcat_s(fechas, to_string(mm).c_str());
						strcat_s(fechas, "/");
						strcat_s(fechas, to_string(yyyy).c_str());
						SendMessageA(GetDlgItem(hWnd, SHOWSP_CB_SPDATE), CB_ADDSTRING, 0, (LPARAM)fechas);
				envio_aux = envio_aux->sig;
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case SHOWSP_BTT_SELEC: {
			/*envio_aux = envio_origen;*/
			GetDlgItemTextA(hWnd, SHOWSP_CB_SPDATE, fechas, sizeof(fechas));
			char aux[4];
			aux[0] = fechas[0];
			aux[1] = fechas[1];
			dd = atoi(aux);
			aux[0] = fechas[3];
			aux[1] = fechas[4];
			mm = atoi(aux);
			aux[0] = fechas[6];
			aux[1] = fechas[7];
			aux[2] = fechas[8];
			aux[3] = fechas[9];
			yyyy = atoi(aux);
			
			while (envio_aux->sig != NULL) {
				if (dd == envio_aux->date.dd && mm == envio_aux->date.mm && yyyy == envio_aux->date.yyyy)
					break;
				else
					envio_aux = envio_aux->sig;
			} 
			//CREACION DE UN NODO AUXILIAR
			envio *enviobuffer = new envio;
			strcpy_s(enviobuffer->calle, envio_aux->calle);
			strcpy_s(enviobuffer->colonia, envio_aux->colonia);
			strcpy_s(enviobuffer->ciudad, envio_aux->ciudad);
			strcpy_s(enviobuffer->estado, envio_aux->estado);
			strcpy_s(enviobuffer->mensaje, envio_aux->mensaje);
			for (i = 0; i < 25; i++)
			{
				strcpy_s(enviobuffer->order[i].producto, envio_aux->order[i].producto);
			}
			enviobuffer->date.dd = envio_aux->date.dd;
			enviobuffer->date.mm = envio_aux->date.mm;
			enviobuffer->date.yyyy = envio_aux->date.yyyy;
			enviobuffer->total = envio_aux->total;
			SetDlgItemTextA(hWnd, SHOWSP_EDIT_CALLE, enviobuffer->calle);
			SetDlgItemTextA(hWnd, SHOWSP_EDIT_COL, enviobuffer->colonia);
			SetDlgItemTextA(hWnd, SHOWSP_EDIT_CIUDAD, enviobuffer->ciudad);
			SetDlgItemTextA(hWnd, SHOWSP_EDIT_ESTADO, enviobuffer->estado);
			SetDlgItemTextA(hWnd, SHOWSP_EDIT_MSG, enviobuffer->mensaje);
///Se crea un nodo auxiliar debido a que imprime info de otros nodos en la ventana
			SendMessageA(GetDlgItem(hWnd, SHOWSP_LB_ORDEN), LB_RESETCONTENT, 0, 0);
			SendMessageA(GetDlgItem(hWnd, SHOWSP_LB_TOTAL), LB_RESETCONTENT, 0, 0);
			for (i = 0; i < 25; i++)
				SendMessageA(GetDlgItem(hWnd, SHOWSP_LB_ORDEN), LB_INSERTSTRING, i, reinterpret_cast<LPARAM>(enviobuffer->order[i].producto));
			SendMessageA(GetDlgItem(hWnd, SHOWSP_LB_TOTAL), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(to_string(enviobuffer->total).c_str()));

			if (yyyy<=fechaHoy.wYear)
				if(mm<=fechaHoy.wMonth)
					if(dd<fechaHoy.wDay)
						SetDlgItemTextA(hWnd, SHOWSP_STATUS, "ENVIADO");
					else
						SetDlgItemTextA(hWnd, SHOWSP_STATUS, "PENDIENTE");
			delete enviobuffer;
			break;
		}
		break;
		}
	}
	return FALSE;
}
BOOL CALLBACK editarenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	int i = 0, j = 0;
	int dd = 0, mm = 0, yyyy = 0;
	char fechas[11];
	SYSTEMTIME fechaHoy;
	ZeroMemory(&fechaHoy, sizeof(fechaHoy));
	GetSystemTime(&fechaHoy);
	envio* enviobuffer = new envio;
	switch (msg)
	{
	case WM_INITDIALOG:
		if (envio_origen == NULL)
			::MessageBox(NULL, L"No hay ordenes agregadas", L"Error", MB_OK || MB_ICONEXCLAMATION);
		else
		{
			quickSort(envio_origen, envio_fin);
			envio_aux = envio_origen;
			while (envio_aux != nullptr) {
				dd = envio_aux->date.dd;
				mm = envio_aux->date.mm;
				yyyy = envio_aux->date.yyyy;
				if (yyyy>=fechaHoy.wYear)
					if (mm>=fechaHoy.wMonth)
						if ((dd<fechaHoy.wDay && mm > fechaHoy.wMonth) || dd>fechaHoy.wDay){
							if (dd < 10) {
								strcpy_s(fechas, "0");
								strcat_s(fechas, to_string(dd).c_str());
							}
							else
								strcpy_s(fechas, to_string(dd).c_str());
							strcat_s(fechas, "/");
							if (mm < 10)
								strcat_s(fechas, "0");
							strcat_s(fechas, to_string(mm).c_str());
							strcat_s(fechas, "/");
							strcat_s(fechas, to_string(yyyy).c_str());
							SendMessageA(GetDlgItem(hWnd, EDITSP_CB_SPDATE), CB_ADDSTRING, 0, (LPARAM)fechas);
						}

				envio_aux = envio_aux->sig;
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case EDITSP_BTT_SELEC: {
			envio_aux = envio_origen;
			GetDlgItemTextA(hWnd, EDITSP_CB_SPDATE, fechas, sizeof(fechas));
			char aux[4];
			aux[0] = fechas[0];
			aux[1] = fechas[1];
			dd = atoi(aux);
			aux[0] = fechas[3];
			aux[1] = fechas[4];
			mm = atoi(aux);
			aux[0] = fechas[6];
			aux[1] = fechas[7];
			aux[2] = fechas[8];
			aux[3] = fechas[9];
			yyyy = atoi(aux);

			while (envio_aux->sig != NULL) {
				if (dd == envio_aux->date.dd && mm == envio_aux->date.mm && yyyy == envio_aux->date.yyyy)
					break;
				else
					envio_aux = envio_aux->sig;
			}
			strcpy_s(enviobuffer->calle, envio_aux->calle);
			strcpy_s(enviobuffer->colonia, envio_aux->colonia);
			strcpy_s(enviobuffer->ciudad, envio_aux->ciudad);
			strcpy_s(enviobuffer->estado, envio_aux->estado);
			strcpy_s(enviobuffer->mensaje, envio_aux->mensaje);
			for (i = 0; i < 25; i++)
			{
				strcpy_s(enviobuffer->order[i].producto, envio_aux->order[i].producto);
			}
			enviobuffer->date.dd = envio_aux->date.dd;
			enviobuffer->date.mm = envio_aux->date.mm;
			enviobuffer->date.yyyy = envio_aux->date.yyyy;
			enviobuffer->total = envio_aux->total;
			SetDlgItemTextA(hWnd, EDITSP_EDIT_CALLE, enviobuffer->calle);
			SetDlgItemTextA(hWnd, EDITSP_EDIT_COL, enviobuffer->colonia);
			SetDlgItemTextA(hWnd, EDITSP_EDIT_CIUDAD, enviobuffer->ciudad);
			SetDlgItemTextA(hWnd, EDITSP_EDIT_ESTADO, enviobuffer->estado);
			SetDlgItemTextA(hWnd, EDITSP_EDIT_MSG, enviobuffer->mensaje);
			SendMessageA(GetDlgItem(hWnd, EDITSP_LB_ORDEN), LB_RESETCONTENT, 0, 0);
			SendMessageA(GetDlgItem(hWnd, EDITSP_LB_TOTAL), LB_RESETCONTENT, 0, 0);
			for (i = 0; i < 25; i++)
				SendMessageA(GetDlgItem(hWnd, EDITSP_LB_ORDEN), LB_INSERTSTRING, i, reinterpret_cast<LPARAM>(enviobuffer->order[i].producto));
			SendMessageA(GetDlgItem(hWnd, EDITSP_LB_TOTAL), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(to_string(enviobuffer->total).c_str()));

			
			break;
		}
		 break;
		case EDIT_BTT_SAVE: {
			GetDlgItemTextA(hWnd, EDITSP_CB_SPDATE, fechas, sizeof(fechas));
			char aux[4];
			aux[0] = fechas[0];
			aux[1] = fechas[1];
			dd = atoi(aux);
			aux[0] = fechas[3];
			aux[1] = fechas[4];
			mm = atoi(aux);
			aux[0] = fechas[6];
			aux[1] = fechas[7];
			aux[2] = fechas[8];
			aux[3] = fechas[9];
			yyyy = atoi(aux);

			GetDlgItemTextA(hWnd, EDITSP_EDIT_CALLE, enviobuffer->calle, sizeof(enviobuffer->calle));
			GetDlgItemTextA(hWnd, EDITSP_EDIT_COL, enviobuffer->colonia, sizeof(enviobuffer->colonia));
			GetDlgItemTextA(hWnd, EDITSP_EDIT_CIUDAD, enviobuffer->ciudad, sizeof(enviobuffer->ciudad));
			GetDlgItemTextA(hWnd, EDITSP_EDIT_ESTADO, enviobuffer->estado, sizeof(enviobuffer->estado));
			GetDlgItemTextA(hWnd, EDITSP_EDIT_MSG, enviobuffer->mensaje, sizeof(enviobuffer->mensaje));
			enviobuffer->date.dd = dd;
			enviobuffer->date.mm = mm;
			enviobuffer->date.yyyy = yyyy;
			Cambio_Envio(enviobuffer);
			escribir_bin();
			delete enviobuffer;

			::MessageBox(NULL, L"Detalles del Envio actualizados", L"System", MB_OK);

			SetDlgItemTextA(hWnd, EDITSP_EDIT_CALLE, "");
			SetDlgItemTextA(hWnd, EDITSP_EDIT_COL, "");
			SetDlgItemTextA(hWnd, EDITSP_EDIT_CIUDAD, "");
			SetDlgItemTextA(hWnd, EDITSP_EDIT_ESTADO, "");
			SetDlgItemTextA(hWnd, EDITSP_EDIT_MSG, "");
			SendMessageA(GetDlgItem(hWnd, EDITSP_LB_ORDEN), LB_RESETCONTENT, 0, 0);
			SendMessageA(GetDlgItem(hWnd, EDITSP_LB_TOTAL), LB_RESETCONTENT, 0, 0);
			break;
		}
			break;
		}
	}
	return FALSE;
}
BOOL CALLBACK eliminarenvio_CB(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Selecciondemenu(hWnd, msg, wParam);
	int i = 0, j = 0;
	int dd = 0, mm = 0, yyyy = 0;
	char fechas[11];
	SYSTEMTIME fechaHoy;
	ZeroMemory(&fechaHoy, sizeof(fechaHoy));
	GetSystemTime(&fechaHoy);
	/*envio_aux = envio_origen;*/
	switch (msg)
	{
	case WM_INITDIALOG:
		if (envio_origen == NULL)
			::MessageBox(NULL, L"No hay ordenes agregadas", L"Error", MB_OK || MB_ICONEXCLAMATION);
		else
		{
			quickSort(envio_origen, envio_fin);
			envio_aux = envio_origen;
			while (envio_aux != nullptr) {
				dd = envio_aux->date.dd;
				mm = envio_aux->date.mm;
				yyyy = envio_aux->date.yyyy;
				if (yyyy >= fechaHoy.wYear)
					if (mm >= fechaHoy.wMonth)
						if ((dd<fechaHoy.wDay && mm > fechaHoy.wMonth) || dd > fechaHoy.wDay) {
							if (dd < 10) {
								strcpy_s(fechas, "0");
								strcat_s(fechas, to_string(dd).c_str());
							}
							else
								strcpy_s(fechas, to_string(dd).c_str());
							strcat_s(fechas, "/");
							if (mm < 10)
								strcat_s(fechas, "0");
							strcat_s(fechas, to_string(mm).c_str());
							strcat_s(fechas, "/");
							strcat_s(fechas, to_string(yyyy).c_str());
							SendMessageA(GetDlgItem(hWnd, DLTSP_CB_SPDATE), CB_ADDSTRING, 0, (LPARAM)fechas);
						}

				envio_aux = envio_aux->sig;
			}
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case DLTSP_BTT_SELEC: {
			envio_aux = envio_origen;
			GetDlgItemTextA(hWnd, DLTSP_CB_SPDATE, fechas, sizeof(fechas));
			char aux[4];
			aux[0] = fechas[0];
			aux[1] = fechas[1];
			dd = atoi(aux);
			aux[0] = fechas[3];
			aux[1] = fechas[4];
			mm = atoi(aux);
			aux[0] = fechas[6];
			aux[1] = fechas[7];
			aux[2] = fechas[8];
			aux[3] = fechas[9];
			yyyy = atoi(aux);

			while (envio_aux->sig != NULL) {
				if (dd == envio_aux->date.dd && mm == envio_aux->date.mm && yyyy == envio_aux->date.yyyy)
					break;
				else
					envio_aux = envio_aux->sig;
			}
			envio* enviobuffer = new envio;
			strcpy_s(enviobuffer->calle, envio_aux->calle);
			strcpy_s(enviobuffer->colonia, envio_aux->colonia);
			strcpy_s(enviobuffer->ciudad, envio_aux->ciudad);
			strcpy_s(enviobuffer->estado, envio_aux->estado);
			strcpy_s(enviobuffer->mensaje, envio_aux->mensaje);
			for (i = 0; i < 25; i++)
			{
				strcpy_s(enviobuffer->order[i].producto, envio_aux->order[i].producto);
			}
			enviobuffer->date.dd = envio_aux->date.dd;
			enviobuffer->date.mm = envio_aux->date.mm;
			enviobuffer->date.yyyy = envio_aux->date.yyyy;
			enviobuffer->total = envio_aux->total;
			SetDlgItemTextA(hWnd, DLTSP_EDIT_CALLE, enviobuffer->calle);
			SetDlgItemTextA(hWnd, DLTSP_EDIT_COL, enviobuffer->colonia);
			SetDlgItemTextA(hWnd, DLTSP_EDIT_CIUDAD, enviobuffer->ciudad);
			SetDlgItemTextA(hWnd, DLTSP_EDIT_ESTADO, enviobuffer->estado);
			SetDlgItemTextA(hWnd, DLTSP_EDIT_MSG, enviobuffer->mensaje);
			SendMessageA(GetDlgItem(hWnd, DLTSP_LB_ORDEN), LB_RESETCONTENT, 0, 0);
			SendMessageA(GetDlgItem(hWnd, DLTSP_LB_TOTAL), LB_RESETCONTENT, 0, 0);
			for (i = 0; i < 25; i++)
				SendMessageA(GetDlgItem(hWnd, DLTSP_LB_ORDEN), LB_INSERTSTRING, i, reinterpret_cast<LPARAM>(enviobuffer->order[i].producto));
			SendMessageA(GetDlgItem(hWnd, DLTSP_LB_TOTAL), LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(to_string(enviobuffer->total).c_str()));

			delete enviobuffer;
			break;
		}
		break;
		case DLTSP_BTT_DLT:
			Baja_Envio();
			SetDlgItemTextA(hWnd, DLTSP_EDIT_CALLE, "");
			SetDlgItemTextA(hWnd, DLTSP_EDIT_COL, "");
			SetDlgItemTextA(hWnd, DLTSP_EDIT_CIUDAD, "");
			SetDlgItemTextA(hWnd, DLTSP_EDIT_ESTADO, "");
			SetDlgItemTextA(hWnd, DLTSP_EDIT_MSG, "");
			SendMessageA(GetDlgItem(hWnd, DLTSP_LB_ORDEN), LB_RESETCONTENT, 0, 0);
			SendMessageA(GetDlgItem(hWnd, DLTSP_LB_TOTAL), LB_RESETCONTENT, 0, 0);
			::MessageBox(NULL, L"Envio Cancelado", L"System", MB_OK || MB_ICONEXCLAMATION);
			escribir_bin();
		break;
		}
	}
	return FALSE;
}