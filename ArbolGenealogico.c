#include<stdio.h>
#include<stdlib.h>
#include<string.h>


typedef struct Pagina{
	//int posicionx;
	//int posiciony;
	struct Claves *inicio;
}pag;
typedef struct Claves{
	//int posicionx;
	//int posiciony;
	struct Claves *siguiente;
	char  Esposo[20];
	pag *hijos;
	int m;
}clav;


pag *aloja_pagina(){
	pag *nuevo=(pag*)malloc(sizeof(pag));
	nuevo->inicio=NULL;
return  nuevo;
}

clav *aloja_clave(char esposo [20]){

	clav *nuevo=(clav*)malloc(sizeof(clav));
	if(!nuevo)
	return NULL;
	strcpy(nuevo->Esposo,esposo);
	nuevo->hijos=NULL;
	nuevo->siguiente=NULL;
	return nuevo;
}

clav **buscar_en_hijos(char padre[20],pag  *pagina); 

 clav **busqueda(char  padre[20] ,clav **clave){
 	if(!(*clave))
 		return NULL;

 	if(strcmp((*clave)->Esposo,padre)==0)
 		return clave;

 	if((*clave)->hijos!=NULL)
 	return buscar_en_hijos(padre,(*clave)->hijos);
 	
 	if((*clave)->siguiente!=NULL)
 	return busqueda(padre,&(*clave)->siguiente);
 	return NULL;
 }

  clav **buscar_en_hijos(char  padre[20], pag  *pagina){
 	if(!pagina)
 		return NULL;
 		return busqueda(padre,&pagina->inicio); //AQui buscamos entre hijos de la lista
 }
clav * Regresar_Inicio_Hijos(pag *pagina){
return pagina->inicio;
}

int  agregar_nodo(clav **clave,char padre[20],char hijo[20])
{

	clav ** Jefe = busqueda(padre, &(*clave));
	clav  *nuevo;
	nuevo=aloja_clave(hijo);
	

	if((*Jefe)->hijos==NULL){
		pag *pag=aloja_pagina();
		pag->inicio=nuevo;
		(*Jefe)->hijos=pag;
		

		return 1;
	}
	else{
		clav *aux=Regresar_Inicio_Hijos((*Jefe)->hijos);
		if((*Jefe)->hijos!=NULL){
			nuevo->siguiente=aux;
			(*Jefe)->hijos->inicio=nuevo;
		
			return 1;
		}
	}
	return 0;
}
void mostrar_SO(clav *clave, int desp,int band);
void mostrar_en_hijos_SO( pag *hijos, int desp)
{
	if(!hijos)
		return;
	if(hijos->inicio!=NULL)
	mostrar_SO(hijos->inicio,desp,0);

}
void mostrar_SO(clav *clave, int desp,int band)
{
	if(!clave)
		return;

	
	
	for (int i = 0; i < desp; ++i)
		printf("%c  ",179);
	if(band==0)
		printf("%c%c%s\n",195,196,clave->Esposo);
	if(band==1)
		printf("%c%c%s\n",192,196,clave->Esposo);
	

	mostrar_en_hijos_SO(clave->hijos, desp+1);
	mostrar_SO(clave->siguiente,desp,0);
	return;
}

int main(){



	int opc;
	char hijo[20];
	char hija[20];
	char padre[20];
	char madre[20];
	clav *clave=NULL;
	pag *Pagina=NULL;
	printf("Ingrese el nombre del padre raiz: ");
	scanf("%s",padre);
	clave=aloja_clave(padre);
	Pagina=aloja_pagina();
	Pagina->inicio=clave;

do{
		
		printf("\n");
		printf("MENU\n");
		printf("1.-Insertar Hijo \n" );
		printf("2.-Mostrar Arbol \n" );
		scanf("%i",&opc);
		
		switch(opc){
			case 1:
				printf("Ingrese el nombre del Hijo: ");
				scanf("%s",hijo);
				if(Pagina)
				{
					printf("Abajo de Padre quieres insertarlo : ");
					scanf("%s",padre);
				}
				if(agregar_nodo(&Pagina->inicio,padre,hijo))
					printf(" 	Dato ingresado con exito\n");
				else
					printf("	Error al ingresar dato\n");
			break;
			case 2:
			printf("----------------------------------------------\n");
			printf("ARBOL GENEALOGICO\n");
			mostrar_SO(Pagina->inicio,0,0);

			printf("----------------------------------------------\n");
		break;
			
		}
	}while(opc!=3);

return 1;

}