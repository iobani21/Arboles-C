#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// RECUERDA PONER NOMBRE DEL STRUCT Y NO EL APODO 
typedef  struct Lista{
  int ID ;
  struct Lista *siguiente;
  struct n_general *Nodo;

}lista;

typedef  struct n_general{
  int dato  ;
  struct Lista *Rama;
  

}Nodos;

typedef struct Cola_s{
	struct Cola_s *Sig;
	Nodos **Nodo;
}eCola;

typedef struct F_Cola
{
	eCola *Frente;
	eCola *Final;
}iCola;

 Nodos **buscar_en_hijos(int dato,lista *Rama); 

 Nodos **busqueda(int dato ,Nodos **Raiz){
 	if(!(*Raiz))
 		return NULL;

 	if((*Raiz)->dato==dato)
 		return Raiz;

 	return buscar_en_hijos(dato,(*Raiz)->Rama);
 }

 Nodos **buscar_en_hijos(int dato,lista  *Rama){
 	if(!Rama)
 		return NULL;
 	Nodos **aux=busqueda(dato,&Rama->Nodo);
 	if(!aux)
 		return buscar_en_hijos(dato,Rama->siguiente); //AQui buscamos entre hijos de la lista
 		return aux;
 }


  int agregar_nodo(int dato, int N_nodos, int Padre, Nodos **Raiz)
{
	Nodos *Nuevo = (Nodos *)malloc(sizeof(Nodos));
	if (!Nuevo)
		return 0;

	Nuevo->dato=dato;
	Nuevo->Rama=NULL;
	if (!(*Raiz))
	{
		(*Raiz)=Nuevo;
		return 1;
	}
	//printf("1\n");
	Nodos ** Jefe = busqueda(Padre, Raiz);
	//printf("2\n");
	if (!Jefe){
		free(Nuevo);
		return 0;
	}

	struct Lista *Lista_N =(struct Lista *)malloc(sizeof(struct Lista));
	if (!Lista_N)
	{
		free(Nuevo);
		return 0;
	}
	if(!(*Jefe)->Rama)
		Lista_N->ID=1;
	else
		Lista_N->ID=(*Jefe)->Rama->ID+1;
	if (Lista_N->ID>N_nodos)
	{
		free(Nuevo);
		free(Lista_N);
		return 0;
	}
	Lista_N->siguiente=(*Jefe)->Rama;
	(*Jefe)->Rama=Lista_N;
	Lista_N->Nodo=Nuevo;
	return 1;

}
void mostrar(Nodos *Raiz, int desp);

void mostrar_en_hijos(struct Lista *Rama, int desp)
{
	if(!Rama)
		return;
	
	mostrar_en_hijos(Rama->siguiente, desp);
	mostrar(Rama->Nodo, desp);
	
}
	
void mostrar(Nodos *Raiz, int desp)
{
	if(!Raiz)
		return;

	mostrar_en_hijos(Raiz->Rama, desp+1);
	
	for (int i = 0; i < desp; ++i)
		printf("\t");
	printf("%i\n", Raiz->dato);
	
}


int main(int argc, char const *argv[])
{
	Nodos *Raiz=NULL;
	iCola B_ancho;
	B_ancho.Frente=NULL;
	B_ancho.Final=NULL;
	int N_nodos,padre,num;

	int dato ;
	int opc;
	int x;

	printf("Ingresa el numero maximo de hijos de cada nodo\n");
	scanf("%i",&N_nodos);
	do{
		printf("\n");
		printf("MENU\n");
		printf("1.-Insertar Nuevo Nodo \n" );
		printf("2.-mostrar arbol\n");
		scanf("%i",&opc);
		
		switch(opc){
			case 1:
			printf("	Ingrese el dato nuevo: ");
			scanf("%i",&num);
			if(Raiz)
			{
				printf("Abajo de que numero quieres insertarlo: ");
				scanf("%i",&padre);
			}
			if(agregar_nodo(num,N_nodos,padre,&Raiz))
				printf(" 	Dato ingresado con exito\n");
			else
				printf("	Error al ingresar dato\n");
		break;
			case 2:
			printf("----------------------------------------------\n");
			mostrar(Raiz,0);
			printf("----------------------------------------------\n");
		break;
			
		}
	}while(opc!=2);

	return 0;

}
