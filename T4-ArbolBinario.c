#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Nodo_Binario{
	int dato;
	struct Nodo_Binario *izquierda;
	struct Nodo_Binario *derecha;
}Arbol;


int insertar(Arbol **ArbolBin, int valor);
Arbol **mas_izquierda_posible(Arbol **ArbolBin);
int eliminar(Arbol **ArbolBin, int elemento);
void mostrar_arbol(Arbol *ArbolBin, int contador);
void buscar(Arbol *ArbolBin, int elemento, int contador);


int main(int argc, char const *argv[]){
	Arbol *ArbolBin = NULL;
	int opcion = 0, valor = 0, resultado, elemento, FE;

	do
	{
		system("cls");
		printf("1. Insertar\n");
		printf("2. Mostrar Arbol\n");
		printf("3. Eliminar\n");
		printf("4. Buscar\n");
		printf("5. Salir\n");
		printf("--> ");
		scanf("%i", &opcion);
		fflush(stdin);
//--------------------------------------------------------------------
		switch(opcion)
		{
			case 1:
				system("cls");
				printf("Ingrese dato\n");
				printf("--> ");
				scanf("%i", &valor);
				fflush(stdin);
				resultado = insertar(&ArbolBin, valor);

				if (resultado == 0)
					printf("No se pudo ingresar\n");
				if (resultado == 1)
					printf("Dato ingresado\n");
				if (resultado == 2)
					printf("El valor ya existe\n");
			break;

			case 2:
				system("cls");
					mostrar_arbol(ArbolBin, 0);
				break;

			case 3:
				printf("Ingrese el elemento a eliminar: \n");
				printf("--> ");
				scanf("%i", &elemento);
				resultado = eliminar(&ArbolBin, elemento);

				if (resultado == 0)
					printf("Dato no encontrado\n");
				if (resultado == 1)
					printf("Dato eliminado\n");
			break;

			case 4:
				printf("Que dato desea buscar?\n");
				printf("--> ");
				scanf("%i", &elemento);
				buscar(ArbolBin, elemento, 1);
			break;
		}

		system("pause");
	}while(opcion != 5);

	return 0;
}


int insertar(Arbol **ArbolBin, int valor){
	if(!(*ArbolBin)){

		Arbol *nuevo_nodo= (Arbol*) malloc(sizeof(Arbol));

		if (!nuevo_nodo){
			printf("Error\n");
			return 0;
		}
		nuevo_nodo->dato = valor;
		nuevo_nodo->izquierda = NULL;
		nuevo_nodo->derecha = NULL;
		(*ArbolBin) = nuevo_nodo;
		return 1;
	}

	else{
		if (valor < (*ArbolBin)->dato)
			insertar(&(*ArbolBin)->izquierda, valor);
		
		if (valor > (*ArbolBin)->dato)
			insertar(&(*ArbolBin)->derecha, valor);
		
		if (valor == (*ArbolBin)->dato)
			return 2;
	}
}

Arbol **mas_izquierda_posible(Arbol **ArbolBin){
	if ((*ArbolBin)->izquierda == NULL)
		return &(*ArbolBin);
	mas_izquierda_posible(&(*ArbolBin)->izquierda);
}


int eliminar(Arbol **ArbolBin, int elemento){
	if ((*ArbolBin) == NULL)
		return 0;

	else if(elemento < (*ArbolBin)->dato)
		eliminar(&(*ArbolBin)->izquierda, elemento);

	else if (elemento > (*ArbolBin)->dato)
		eliminar(&(*ArbolBin)->derecha, elemento);

	else if(elemento == (*ArbolBin)->dato){
		Arbol *auxiliar, **auxiliar2;
			// Si hay izquierda y derecha 
		if((*ArbolBin)->izquierda && (*ArbolBin)->derecha){
			auxiliar2 = mas_izquierda_posible(&(*ArbolBin)->derecha);
			auxiliar= (*auxiliar2);
			(*auxiliar2) = (*auxiliar2)->derecha;
			auxiliar->derecha  = (*ArbolBin)->derecha;
			auxiliar->izquierda = (*ArbolBin)->izquierda;
			free((*ArbolBin));
			(*ArbolBin) = auxiliar;
		}
			// Si solo hay izquierda
		else if((*ArbolBin)->izquierda && !(*ArbolBin)->derecha){
			*ArbolBin = (*ArbolBin)->izquierda;
			(*ArbolBin)->izquierda = NULL;
		}
			// Si solo hay derecha 
		else if((*ArbolBin)->derecha && !(*ArbolBin)->izquierda){
			*ArbolBin = (*ArbolBin)->derecha;
			(*ArbolBin)->derecha = NULL;	
		}
			// Si no hay ninguno 
		else if(!(*ArbolBin)->izquierda && !(*ArbolBin)->derecha)
			(*ArbolBin) = NULL;
		return 1;
	}
}

void mostrar_arbol(Arbol *ArbolBin, int contador){
	if (ArbolBin == NULL)
		return;

	else{
		mostrar_arbol(ArbolBin->derecha, contador+1);
		for (int i = 0; i < contador; i++)
			printf("\t");
		printf("%i\n", ArbolBin->dato);
		mostrar_arbol(ArbolBin->izquierda, contador+1);
		}
}

void buscar(Arbol *ArbolBin, int elemento, int contador){
	if (ArbolBin == NULL)
		return;
		

	else if(elemento < ArbolBin->dato)
		buscar(ArbolBin->izquierda, elemento, contador+1);

	else if (elemento > ArbolBin->dato)
		buscar(ArbolBin->derecha, elemento, contador+1);

	else if(elemento == ArbolBin->dato)
		printf("El elemento esta en el piso %i\n", contador);
}