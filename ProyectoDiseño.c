#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define ORDEN 2





struct Clave 
{
    struct Paciente *paciente;
    struct pagina *izq;
    struct pagina *der;   
};

struct elemento_pagina //lista de nodos
{
    struct Clave *clave;
    struct elemento_pagina *sig;
};
struct Paciente{
    int id;
    char nom[20];
    char app[20];
    char apm[20];
    int  edad;
    int  accion;   // 1=Insercion  2=Eliminacion 
};

struct pagina //esto conceptualmente es una pagina
{

    struct elemento_pagina * inicio; 
};

struct pagina * aloja_pagina() //Reservar Memoria  Pagina
{
    return (struct pagina * ) malloc (sizeof(struct pagina));
}

struct elemento_pagina * aloja_elemento_pagina() //Reservar Memoria Elemneto Pagina
{
    return (struct elemento_pagina * ) malloc (sizeof(struct elemento_pagina));
}

struct Clave * aloja_nodo (struct Paciente *pac) //Rerservar MeMoria Elemento Clave
{
    struct Clave * nuevo = (struct Clave *) malloc (sizeof(struct Clave));
    if (!nuevo)
        return NULL;
    nuevo->paciente = pac;
    nuevo->izq = NULL;
    nuevo->der = NULL;
    return nuevo;
}

struct Paciente *aloja_paciente(int id,char nom[20],char app[20],char apm[20],int edad,int accion){
    
    struct Paciente * nuevo = (struct Paciente *) malloc (sizeof(struct Paciente));
    if(!nuevo)
        return NULL;
    nuevo->id=id;
    strcpy(nuevo->nom,nom);
    strcpy(nuevo->app,app);
    strcpy(nuevo->apm,apm);
    nuevo->edad=edad;
    nuevo->accion=accion;
    return nuevo;
}

int  Comparar_Claves_Eliminacion(struct elemento_pagina ** inicio, int id,int *band,struct elemento_pagina *aux,struct pagina **principal); // Busca entre las claves el elemento que se quiere eliminar
int  Buscar_Pagina_Eliminacion(struct pagina **raiz, int id ,int *band, struct pagina **principal);// Busca en las paginas el elemento que se quiere eliminar
int cuenta_elementos_pag(struct elemento_pagina *raiz); //Cuenta los Elementos de una Pagina
int Agregar_Paciente(struct pagina **raiz, struct Paciente *pac , struct elemento_pagina ** respuesta); //Insercion del Paciente al arbol y si la rama supera a 2*Orden la rama se parte 
void mostrar_SO(struct elemento_pagina  *elempag, int desp,int band,int band2); // Recorre las paginas del arbol 
void mostrar_en_hijos_SO( struct pagina *hijos, int desp,int hoja,int band); // Recore las claves del arbol y muestra las hojas
void Cargar_Pacientes(FILE *flujo,struct pagina **raiz,struct elemento_pagina **res); // Carga los datos guardados en el archivo BIN 
struct Paciente*  Buscar_Pagina(struct pagina *raiz, int id ,int desp); // Recorre las paginas en busca del elemento  y  retorna el paciente buscado 
struct Paciente*  Comparar_Claves(struct elemento_pagina * inicio, int id,int desp);//Recorre las claves en busca del elementos buscado y muestra los datos que se compararon y retorna el paciente buscado
int reinsertar_en_pag(struct elemento_pagina ** inicio, struct elemento_pagina *dato);//Inserta un elemento  en una pagina
struct elemento_pagina* Final_Pag(struct elemento_pagina *inicio);//Retorna la estrucura que esta al final de una pagina
void Fusionar_izq_der(struct pagina **izq, struct pagina **der);//Fusiona las Paginas hijo
struct elemento_pagina* Quitar_Final(struct elemento_pagina **clave);//Busca el elemento final de la pagina y lo retira de la lista
int  Verificar_elementos(struct elemento_pagina **inicio,int ndatos,int *band2);//Verifica si la pagina contiene menos de Orden elementos
struct elemento_pagina ** get_centro(struct elemento_pagina **raiz, int cont); // Retorna el elemento pagina ubicado en medio de la pagina (Orden+1)
int inserta_elemento_pagina(struct elemento_pagina ** inicio, struct Paciente *pac, struct elemento_pagina ** respuesta);//inserta el elemento nuevo en una pagina






int cuenta_elementos_pag(struct elemento_pagina *raiz)
{
    if (!raiz) return 0;
    return 1 + cuenta_elementos_pag(raiz->sig);
}


int inserta_elemento_pagina(struct elemento_pagina ** inicio, struct Paciente *pac, struct elemento_pagina ** respuesta)
{
    if (!(*inicio)) // si la lista está vacía
    {
        if(!((*inicio) = aloja_elemento_pagina())) // insertar la lista de nodos
            return 0;

        if(!((*inicio)->clave = aloja_nodo(pac))) // creamos el nodo en el elemento anterior de lista
            return 0;

        (*inicio)->sig = NULL;
        return 1; // success
    }

    if((*inicio)->clave->paciente->id==pac->id && (*inicio)->clave->der==NULL && (*inicio)->clave->izq==NULL)
    {
        printf("Id ya  cargado en arbol \n");
        return 0;
    }

    if ((*inicio) -> clave->paciente ->id > pac->id) //del lado izq
    {
        if((*inicio)->clave->izq) //si tiene hijo entonces va para abajo
        {
            //insertar del lado izq.
            return Agregar_Paciente(&((*inicio) -> clave ->izq), pac, respuesta);
        }

        //insertar aqui en la lista de nodos
        struct elemento_pagina * nuevo = NULL;        
        if(!(nuevo = aloja_elemento_pagina())) // insertar la lista de nodos
            return 0;
        if(!(nuevo->clave = aloja_nodo(pac))) // creamos el nodo en el elemento anterior de lista
            return 0; 
        nuevo->sig = (*inicio);    
        (*inicio) = nuevo;
        // si de insertarlo se pasa de 2d   
        return 1;
    }
    if (!(*inicio)->sig) // si el siguiente es el fin de la lista
        if ((*inicio)->clave->der) // y tiene hijo a la derecha
            return Agregar_Paciente(&((*inicio) -> clave ->der), pac, respuesta); // vas para abajo del lado derecho
    return inserta_elemento_pagina(&(*inicio)->sig, pac, respuesta); //entramos en la recursión
}



struct elemento_pagina ** get_centro(struct elemento_pagina **raiz, int cont)
{
    if (!raiz)
        return NULL;
    if (cont == ORDEN)
        return raiz;
    return get_centro(&(*raiz)->sig,cont+1);
}




int insertar_en_esta_pag(struct elemento_pagina ** inicio, struct elemento_pagina *clave, int *bandera)
{
    if (!(*inicio))// Acomodar al final
    {
        (*inicio) = clave;
        clave->sig = NULL;
        (*bandera)=1;
        return 1;
     
    }
    if(((*inicio)->clave->paciente->id > clave->clave->paciente->id)) //ACOMODAR al inicio o en medio11
    {
        clave->sig = (*inicio);
        (*inicio) = clave;        
        (*inicio)->sig->clave->izq=(*inicio)->clave->der;
        *bandera=1;
        return 2;
     
    }
 insertar_en_esta_pag(&(*inicio)->sig, clave,bandera);
    if(*bandera==1)  
    {
        (*inicio)->clave->der=(*inicio)->sig->clave->izq;
        *bandera=0;
    }
return 1;
}


int Agregar_Paciente(struct pagina **raiz, struct Paciente *pac , struct elemento_pagina ** respuesta){
     if (!(*raiz))
    {
        *raiz = aloja_pagina(); // se reserva memoria para la página (raiz)
        if (!*(raiz))
            return 0;       
        if (!inserta_elemento_pagina(&(*raiz)->inicio, pac, respuesta))   
            return 0;
     
        return 1;
    }
    int res = inserta_elemento_pagina(&(*raiz)->inicio, pac, respuesta); 
  

    if (*respuesta)
    {
        int x=0;
        int *bandera;
        bandera=&x;
        insertar_en_esta_pag(&(*raiz)->inicio, *respuesta,bandera); 
        (*respuesta)=NULL;
    }
    int m = cuenta_elementos_pag((*raiz)->inicio);
    if (m > 2*ORDEN)
    {
        printf("****Operacion Importante****\n");
        printf("Pagina  supero los 2D elementos\n");

        //partir la pagina
        struct elemento_pagina ** centro = get_centro(&(*raiz)->inicio,0); 
        if (!centro)
            return 0;
        struct pagina *izq= aloja_pagina();
        struct pagina *der= aloja_pagina();
        struct elemento_pagina *copia_elempagina =aloja_elemento_pagina();
        struct Paciente *copia_paciente =aloja_paciente((*centro)->clave->paciente->id," "," "," ",0,0);
        printf("Pagina partida la clave %i se clono y subio  \n",(*centro)->clave->paciente->id );
        struct Clave * nuevo = (struct Clave *) malloc (sizeof(struct Clave));
        nuevo->paciente=copia_paciente;
        copia_elempagina->sig=NULL;        
        copia_elempagina->clave=nuevo;        
        if (!(izq || der))
            return 0;
        copia_elempagina->clave->izq = izq;
        copia_elempagina->clave->der = der;
        izq->inicio = (*raiz)->inicio;
        if((*centro)->clave->izq!=NULL && (*centro)->clave->der!=NULL)               
            der->inicio=(*centro)->sig;                  
        if((*centro)->clave->izq==NULL && (*centro)->clave->der==NULL)  
             der->inicio = (*centro);

        (*raiz)->inicio->sig->sig=NULL;
        (*raiz)->inicio = copia_elempagina;
        *respuesta = (*raiz)->inicio;
        printf("***************************\n");
        return 1;
    }
    return res;
}

void mostrar_en_hijos_SO( struct pagina *hijos, int desp,int hoja,int band)
{
    if(!hijos)     
    return;

    for (int i = 0; i < desp+1; ++i)
        printf(" |");
    if(hijos->inicio!=NULL)     
        if(band==1)
            {
                if(hijos->inicio->clave->izq==NULL)
                printf("%c%c Hoja %i \n",192,196,hoja );
                else
                 printf("%c%c Rama %i \n",192,196,hoja );  
            }
        if(band==0)
            printf(" Raiz\n");
    mostrar_SO(hijos->inicio,desp,0,1);

}
void mostrar_SO(struct elemento_pagina *elempag, int desp,int band,int band2)
{
    if(!elempag)
    return;
    int aux=desp;  
    
    if(elempag->clave->izq==NULL && elempag->clave->izq==NULL)
    {
        for (int i = 0; i < desp+1; ++i)
        printf(" |");
        printf(" |-%i %s %s  \n",elempag->clave->paciente->id,elempag->clave->paciente->nom,elempag->clave->paciente->app);           
    }
    else
    {
           for (int i = 0; i < desp; ++i)
        printf(" |  ");          
        printf(" %c%c%i \n",195,196,elempag->clave->paciente->id ); 
    }
    mostrar_en_hijos_SO(elempag->clave->izq,aux+1,1,1);
    mostrar_en_hijos_SO(elempag->clave->der, aux+1,2,1);
    mostrar_SO(elempag->sig,desp,0,0);
    return;
}


 void Cargar_Pacientes(FILE *flujo,struct pagina **raiz,struct elemento_pagina **res)
{
    int x;
    struct Paciente  paciente;
    struct Paciente * pac;
    fread(&paciente,sizeof(paciente),1,flujo);
    int validar;
    if(feof(flujo)==0)
    {     
        pac=aloja_paciente(paciente.id,paciente.nom,paciente.app,paciente.apm,paciente.edad,paciente.accion);
        if(pac->accion==1)
        {
            printf("Insertando en arbol ID:%i \n",paciente.id );
            validar=Agregar_Paciente(&(*raiz),pac,&(*res));
            (*res)=NULL;   
        }
        if(pac->accion==2)
        {
            printf("Eliminando en arbol ID:%i \n",paciente.id);
            int bandera;
            int  *band=&bandera;
            *band=0;
            validar=Buscar_Pagina_Eliminacion(&(*raiz),pac->id,band,&(*raiz));            
        }
     Cargar_Pacientes(flujo,raiz,res);  
     return;
    }    
    return;
}


struct Paciente*  Comparar_Claves(struct elemento_pagina * inicio, int id,int desp)
{
    if (!(inicio)) // si la lista está vacía
       return NULL;

    if((inicio)->clave->paciente->id==id && (inicio)->clave->der==NULL && (inicio)->clave->izq==NULL)
    {
        printf("->%i\n",inicio->clave->paciente->id);
        return inicio->clave->paciente;
    }

    if ((inicio) -> clave->paciente ->id >id) //del lado izq
    {         
        if(inicio->clave->izq) //si tiene hijo entonces va para abajo
        {
            printf("->%i\n",inicio->clave->paciente->id);
            return Buscar_Pagina(inicio -> clave ->izq, id,desp+1);
        }   
    }
   
    if (!(inicio)->sig) // si el siguiente es el fin de la lista
    {
        printf("->%i\n",inicio->clave->paciente ->id);
        if ((inicio)->clave->der) // y tiene hijo a la derecha
        return Buscar_Pagina(inicio -> clave ->der, id,desp+1); // vas para abajo del lado derecho
    }
    if(inicio->sig!=NULL)
        printf("->%i",inicio->clave->paciente->id );
    return Comparar_Claves(inicio->sig, id,desp+1); //entramos en la recursión
}


struct Paciente* Buscar_Pagina(struct pagina *raiz, int id ,int desp){
     if (!(raiz))
     return NULL;
     desp++;
     for (int i = 0; i < desp+1; ++i)
            printf(" %c",179);
     return  Comparar_Claves((raiz)->inicio, id,desp);      
}

int reinsertar_en_pag(struct elemento_pagina ** inicio, struct elemento_pagina *dato)
{
    if (!(*inicio))
    {
        (*inicio) = dato;
        dato->sig = NULL;
        return 1;
    }
    if(((*inicio)->clave->paciente->id > dato->clave->paciente->id))
    {
        dato->sig = (*inicio);
        (*inicio) = dato;
        return 2;
    }
    reinsertar_en_pag(&(*inicio)->sig, dato);
     return 0;
}

struct elemento_pagina* Final_Pag(struct elemento_pagina *inicio)
{
    if(inicio->sig!=NULL)
        return Final_Pag(inicio->sig);
        return inicio;
}

void Fusionar_izq_der(struct pagina **izq, struct pagina **der)
{
    struct elemento_pagina *aux;
  
   
    aux=Final_Pag((*izq)->inicio); // Se obtiene el inicio de la hoja izq 
    if(aux->clave->der!=NULL)      //Se verifica si es una hoja 
    {
        aux->sig=(*der)->inicio;        
        Fusionar_izq_der(&aux->clave->der,&(*der)->inicio->clave->izq); // Se fusionan las hojas hijos si es una rama                           
        return;      
    }
    aux->sig=(*der)->inicio;    
    (*der)->inicio=(*izq)->inicio;
    return ;
}

struct elemento_pagina* Quitar_Final(struct elemento_pagina **clave)
{
    if((*clave)->sig!=NULL)
        return Quitar_Final(&(*clave)->sig);
        struct elemento_pagina *aux;
        aux=(*clave);
        (*clave)=NULL;
        return aux;
}    



int  Verificar_elementos(struct elemento_pagina **inicio,int ndatos,int *band2)
{

    int x;
    struct elemento_pagina *aux;
    struct elemento_pagina *aux2;
    struct pagina *aux3;
    if(!(*inicio))
        return 1;

    if((*inicio)->clave->izq==NULL &&(*inicio)->clave->der==NULL)
        return 0;

    if((*inicio)->sig!=NULL)
    {        
        (*inicio)->clave->der=(*inicio)->sig->clave->izq;            
    }
    int m1 = cuenta_elementos_pag((*inicio)->clave->izq->inicio);
    int m2 = cuenta_elementos_pag((*inicio)->clave->der->inicio);
    //printf("m1 %i \n",m1 );
    //printf("m2 %i \n",m2 );
    if(m1==2  && m2==2)
     return  Verificar_elementos(&(*inicio)->sig,ndatos,band2);
    //Casos Bases de Reestructuracion
    if(m1<ORDEN) // Si la hoja izq es menor a Orden
    {
        if(m2>=ORDEN+1) // y m2 es Almenos igual Orden+1
        {       
            if(ndatos==1) // y existe mas de una clave hermana
            {
                printf("****Operacion Importante****\n");
                printf("El hijo Izquierdo de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );
                printf("No existen claves hermanas\n");
                printf("Hoja derecha tiene %i elementos \n",m2 );   
                (*inicio)->clave->paciente->id=(*inicio)->clave->der->inicio->sig->clave->paciente->id;
                printf("La clave %i subio \n",(*inicio)->clave->der->inicio->sig->clave->paciente->id);
                aux=Final_Pag((*inicio)->clave->izq->inicio);
                aux2=(*inicio)->clave->der->inicio;
                printf("La clave %i paso a la hoja izquierda  \n",aux2->clave->paciente->id);
                (*inicio)->clave->der->inicio=(*inicio)->clave->der->inicio->sig;
                aux2->sig=NULL;
                aux->sig=aux2;
                printf("***************************\n");
                return 2;
            }
        else{   //Si solo existe una clave 
             if((*inicio)->sig==NULL) // Si estamos al final de la pagina o es la unica
                {
                    if(m2==ORDEN+1)// Si la hoja derecha tiene Orden+1 elementos 
                    {    
                        printf("****Operacion Importante****\n");
                        printf("El hijo Izquierdo de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );
                        printf("Estamos al final de la pagina\n");
                        (*inicio)->clave->paciente->id=(*inicio)->clave->der->inicio->sig->clave->paciente->id;
                        printf("Hoja derecha tiene %i elementos\n",m2 );
                        printf("La clave %i subio \n",(*inicio)->clave->der->inicio->sig->clave->paciente->id);
                        aux=Final_Pag((*inicio)->clave->izq->inicio);              
                        aux2=(*inicio)->clave->der->inicio;
                        printf("La clave %i paso a la hoja 1  \n",aux2->clave->paciente->id);
                        (*inicio)->clave->der->inicio=(*inicio)->clave->der->inicio->sig;
                        aux2->sig=NULL;
                        aux->sig=aux2;
                        printf("****************************\n");
                        return 3;
                    }
                    if(m2==ORDEN+2) // Si la hoja derecha tiene Orden+2 elementos
                    {
                        printf("****Operacion Importante****\n");
                        printf("El hijo Izquierdo de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );
                        (*inicio)->clave->paciente->id=(*inicio)->clave->der->inicio->sig->clave->paciente->id;
                        printf("Hoja derecha tiene %i elementos\n",m2 );
                        printf("La clave %i subio \n",(*inicio)->clave->der->inicio->sig->clave->paciente->id);
                        aux=Final_Pag((*inicio)->clave->izq->inicio);              
                        aux2=(*inicio)->clave->der->inicio;
                        printf("La clave %i paso a la hoja 1  \n",aux2->clave->paciente->id);
                        (*inicio)->clave->der->inicio=(*inicio)->clave->der->inicio->sig;
                        aux2->sig=NULL;
                        aux->sig=aux2;       
                        printf("****************************\n");  
                        return 4;
                    }
                }
             if((*inicio)->sig!=NULL) // Si no estamos al final de la pagina
                {     
                    printf("****Operacion Importante****\n");
                    printf("El hijo Izquierdo de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );          
                    (*inicio)->clave->paciente->id=(*inicio)->clave->der->inicio->sig->clave->paciente->id;
                    printf("Hoja derecha tiene %i elementos\n",m2 );
                    aux=Final_Pag((*inicio)->clave->izq->inicio);              
                    aux2=(*inicio)->clave->der->inicio;
                    printf("La clave %i paso a la hoja 1  \n",aux2->clave->paciente->id);
                    (*inicio)->clave->der->inicio=(*inicio)->clave->der->inicio->sig;
                    aux2->sig=NULL;
                    aux->sig=aux2;
                    printf("****************************\n");
                    return 5;                
                }
            }
        }
       if(m2<=ORDEN+1) // Si  la hoja derecha es menor o igual a Orden+1
        {
            if(ndatos!=1) // Si existe mas de una clave 
            {
                if((*inicio)->sig!=NULL)// Y No estamos al final de la pagina
                {
                    printf("****Operacion Importante****\n");
                    printf("El hijo Izquierdo de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );                
                    printf("FUSIONANDO PAGINAS HIJO\n");
                    Fusionar_izq_der(&(*inicio)->clave->izq,&(*inicio)->clave->der);
                    aux=(*inicio);
                    (*inicio)->sig->clave->izq->inicio=(*inicio)->clave->izq->inicio;
                    (*inicio)=(*inicio)->sig;                     
                    printf("Se elimino la clave %i \n",aux->clave->paciente->id );
                    free(aux);
                    printf("****************************\n");
                    return 6;
                }
             else // entonces estamos al final de la pagina
                {
                    if(m1>ORDEN) //Si la hoja izquierda tiene mas de 2 elementos
                    {  
                        aux=Quitar_Final(&(*inicio)->clave->izq->inicio);
                        (*inicio)->clave->paciente->id=aux->clave->paciente->id;                                
                        aux->sig=(*inicio)->clave->der->inicio;
                        (*inicio)->clave->der->inicio=aux;
                        (*inicio)->sig=NULL; 
                        return 7;
                    }
                    else // Sino la hoja izquierda tiene solo dos elementos
                    {
                        printf("****Operacion Importante****\n");
                        printf("El hijo Izquierdo de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );
                        printf("existen claves hermanas\n");                
                        printf("FUSIONANDO PAGINAS HIJO\n");
                        Fusionar_izq_der(&(*inicio)->clave->izq,&(*inicio)->clave->der);
                        aux=(*inicio);
                        printf("Eliminando %i \n",aux->clave->paciente->id );  
                        printf("****************************\n");                                          
                        (*inicio)=NULL;
                        free(aux);
                        return 8;
                    }            
                }                          
            }
        else{ // Si solo existe una clave
                printf("****Operacion Importante****\n");
                printf("El hijo Izquierdo de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );
                printf("No existen claves hermanas\n");                
                printf("FUSIONANDO PAGINAS HIJO\n");
                Fusionar_izq_der(&(*inicio)->clave->izq,&(*inicio)->clave->der);
                aux=(*inicio);
                printf("Eliminando %i \n",aux->clave->paciente->id );
                printf("Subiendo Pagina Fusionada \n");
                printf("****************************\n");
                (*inicio)=(*inicio)->clave->izq->inicio;
                free(aux);
                return 9;
            }
        }
    }

    if(m2<ORDEN) // Si la hoja derecha tiene menos de Orden elementos
    {
        if(m1>=ORDEN+1)     // Si la hoja izquierda tiene al mmenos 3 claves
        {
            if(ndatos>1)    //Si hay mas de una clave
            {       
                printf("****Operacion Importante****\n");
                printf("El hijo Derecho de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );     
                printf("Hoja izquierda tiene %i elementos\n",m1 );
                aux=Quitar_Final(&(*inicio)->clave->izq->inicio);
                printf("Clave %i pasa a la hoja derecha y se copia como clave padre  \n",aux->clave->paciente->id ); 
                (*inicio)->clave->paciente->id=aux->clave->paciente->id;
                aux->sig=(*inicio)->clave->der->inicio;
                (*inicio)->clave->der->inicio=aux;
                printf("****************************\n");
                return 10;
            }
            if(m1>=ORDEN) // si la hoja izquierda  tiene mas de 2 elementos
            {
                if((*inicio)->clave->izq->inicio->clave->izq==NULL) // si la pagina izquierda es una hoja
                {
                    printf("****Operacion Importante****\n");
                    printf("El hijo Derecho de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );  
                    printf("Hoja izquierda tiene %i elementos\n",m1 );
                    aux=Quitar_Final(&(*inicio)->clave->izq->inicio);
                    printf("La clave %i se copio como padre y se paso a Derecha \n",aux->clave->paciente->id );                    
                    (*inicio)->clave->paciente->id=aux->clave->paciente->id;
                    aux->sig=(*inicio)->clave->der->inicio;
                    (*inicio)->clave->der->inicio=aux;
                    printf("****************************\n");
                    
                    return 11;
                }
            else{  //la pagina izquierda no es una hoja
                    printf("****Operacion Importante****\n");
                    printf("El derecho  de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );
                    printf("No existen claves hermanas\n");                
                    printf("FUSIONANDO PAGINAS HIJO\n");
                    Fusionar_izq_der(&(*inicio)->clave->izq,&(*inicio)->clave->der);
                    aux=(*inicio);
                    printf("Eliminando %i \n",aux->clave->paciente->id );
                    printf("Subiendo Pagina Fusionada \n");
                    printf("****************************\n");
                    (*inicio)=(*inicio)->clave->izq->inicio;
                    free(aux);
                    return 12;            
                }          
            }
        }
        if(ndatos==1 && (*inicio)->sig==NULL) // Si  solo hay una clave y estamos al final de la pagina
        {               
            printf("****Operacion Importante****\n");
            printf("El derecho  de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );
            printf("No existen claves hermanas\n");                
            printf("FUSIONANDO PAGINAS HIJO\n");
            Fusionar_izq_der(&(*inicio)->clave->izq,&(*inicio)->clave->der);
            aux=(*inicio);
            printf("Eliminando %i \n",aux->clave->paciente->id );
            printf("Subiendo Pagina Fusionada \n");
            printf("****************************\n");
            (*inicio)=(*inicio)->clave->izq->inicio;
            free(aux);
           
            return 13;            
        }
        if(ndatos>1 && (*inicio)->sig==NULL) // Si existen mas de una clave y estamos al final de la pagina
        {
            printf("****Operacion Importante****\n");
            printf("El hijo derecho de la Clave %i es  menor a  ORDEN: \n",(*inicio)->clave->paciente->id );                
            printf("FUSIONANDO PAGINAS HIJO\n");
            aux=(*inicio);
            Fusionar_izq_der(&(*inicio)->clave->izq,&(*inicio)->clave->der);
            printf("Eliminando clave padre %i \n",aux->clave->paciente->id );
            printf("****************************\n");
            (*inicio)=NULL;
            free(aux);
            return 14;
        }    
    }
 return  Verificar_elementos(&(*inicio)->sig,ndatos,band2);  // Las paginas hijo de la clave estan correctas evaluando siguiente clave
}


int  Comparar_Claves_Eliminacion(struct elemento_pagina ** inicio, int id,int *band,struct elemento_pagina *aux,struct pagina **principal)
{
    aux=(*inicio)->sig;
    struct elemento_pagina *aux2 =(*inicio);
    if (!(inicio)) // si la lista está vacía
    {
       return 0;
    }

    if((*inicio)->clave->paciente->id==id && (*inicio)->clave->der==NULL && (*inicio)->clave->izq==NULL)
    {
 
        printf("Elemento encontrado\n");
        (*inicio)=aux;    
        free(aux2);
        aux=NULL;

        return 1;
    }

    if ((*inicio) -> clave->paciente ->id >id) //del lado izq
    {         
        if((*inicio)->clave->izq) //si tiene hijo entonces va para abajo
        {
            return Buscar_Pagina_Eliminacion(&(*inicio) -> clave ->izq, id,band,&(*principal));
        }   
    }
   
    if (!(*inicio)->sig) // si el siguiente es el fin de la lista
    {           
        if ((*inicio)->clave->der) // y tiene hijo a la derecha
            return Buscar_Pagina_Eliminacion(&(*inicio) -> clave ->der, id,band,&(*principal)); // vas para abajo del lado derecho
    }
    
    if((*inicio)->sig!=NULL)
    return Comparar_Claves_Eliminacion(&(*inicio)->sig, id,band,aux,&(*principal)); //entramos en la recursión
}



int  Buscar_Pagina_Eliminacion(struct pagina **raiz, int id ,int *band,struct pagina **principal){
    if (!(*raiz))
        return 0;
    int y;
    int *band2=&y;
    struct elemento_pagina *aux;
    int  res=Comparar_Claves_Eliminacion(&(*raiz)->inicio, id,band,NULL,&(*principal));
    if(((*raiz)->inicio)==NULL)
    {
        return 1; 
    }
    int m=cuenta_elementos_pag((*raiz)->inicio);

        if(*band==1)
        {
         
           
            int x= Verificar_elementos(&(*raiz)->inicio,m, band2);
            //printf("Metodo de eliminacion %i \n",x );
            if((*raiz)->inicio != (*principal)->inicio  || m>1)
            {    
                
                if(x==0)
                {
                    printf("****Operacion Importante****\n");
                    printf("Los hijos de la Clave %i son menores a  ORDEN: \n",(*raiz)->inicio->clave->paciente->id );                
                    printf("FUSIONANDO PAGINAS HIJO\n");
                    Fusionar_izq_der(&(*raiz)->inicio->clave->izq,&(*raiz)->inicio->clave->der);
                    aux=(*raiz)->inicio;
                    (*raiz)->inicio=(*raiz)->inicio->sig;  
                    *band=0;
                }
             *band=0;
            }
        }

        m=cuenta_elementos_pag((*raiz)->inicio);
 
        if(m<ORDEN*2)
        {
            *band=1;
        }  
    return res;
}

int main(){

    struct pagina * raiz=NULL;
    struct Paciente * paciente;
    struct elemento_pagina *res = NULL;
    int id;
    char nom[20];
    char app[20];
    char apm[20];
    int edad;
    int validar;
    int opc;
    int accion;
    struct Paciente *p;
    struct Paciente pac;
    FILE *flujo;
    clock_t t_init,t_fin;
    flujo=fopen("Pacientes.bin","rb"); 
    if(flujo!=NULL)
    {
        printf("Archivo encontrador cargando....\n");
        Cargar_Pacientes(flujo,&raiz,&res);
        fclose(flujo);
        printf("Arbol Cargado Exitosamente\n");
    }
   system("clear");
  
 do{
        res=NULL;
        printf("\n");
        printf("----------------------------------------------\n");
        printf("MENU\n");
        printf("1.-Insertar Paciente \n" );
        printf("2.-Mostrar  Arbol\n");
        printf("3.-Buscar Elemento\n");
        printf("4.-Eliminar Paciente\n");
        printf("5.-Salir\n");
        scanf("%i",&opc);
        printf("------------------------------------------\n");
        system("clear");
        switch(opc)
            {
                case 1:
                    printf("Ingresa el nombre del paciente\n");
                    scanf("%s",nom);
                    fflush(stdin);
                    printf("Ingresa el apellido paterno del paciente\n");
                    scanf("%s",app);
                    fflush(stdin);
                    printf("Ingresa el apellido materno del paciente\n");
                    scanf("%s",apm);
                    fflush(stdin);
                    printf("Ingresa la edad del paciente\n");
                    scanf("%i",&edad);
                    fflush(stdin);
                    printf("Ingresa el id del paciente\n");
                    scanf("%i",&id);
                    fflush(stdin);
                    paciente=aloja_paciente(id,nom,app,apm,edad,1);
                    validar=Agregar_Paciente(&raiz,paciente,&res);
                    if(validar==1)
                    {
                        pac=*paciente;
                        flujo =fopen("Pacientes.bin","ab");
                        fwrite(&pac,sizeof(pac),1,flujo);                   
                        fclose(flujo);
                        printf("Paciente Registrado\n");
                    }
                    else
                    {
                       printf("Paciente No Registrado\n");
                    }
                    fflush(stdin);
                break;
                case 2:
                    if(raiz!=NULL)
                    {
                        printf("----------------------------------------------\n");
                        printf("ARBOL B+ PACIENTES\n");
                        mostrar_en_hijos_SO(raiz,0,1,0);                  
                        printf("----------------------------------------------\n");
                    }
                    else
                    {
                        printf("Arbol Vacio\n");
                    }
                break;
                case 3:
                    printf("Ingresa el ID del elemento que buscas\n");
                    scanf("%i",&id);
                    if(raiz!=NULL && raiz->inicio!=NULL)
                    {
                    printf("Camino De Busqueda\n");
                    t_init=clock();
                    p=Buscar_Pagina(raiz,id,0);
                    t_fin=clock();
                    double secs =(double)(t_fin-t_init)/CLOCKS_PER_SEC;
                    if(p!=NULL)
                    {
                        printf("-----------DATOS-------------------------\n");
                        printf("ID:%i \n",p->id );
                        printf("Nombre:%s \n",p->nom );
                        printf("APP:%s \n",p->app );
                        printf("APM:%s \n",p->apm );
                    }
                    else
                    {
                        printf("\n ID no encontrado en arbol \n");
                    }
                    printf(" Tiempo de Busqueda : %.16g milisegundos\n",secs *1000.0 );
                    }
                    else{
                        printf("Arbol Vacio\n");
                    }

                break;
                case 4:
                    printf("Ingresa el ID que quieres eliminar\n");
                    scanf("%i",&id);

                    int bandera;
                    int  *band=&bandera;
                    *band=0;

                    if(raiz!=NULL && raiz->inicio!=NULL)
                    {

                    validar=Buscar_Pagina_Eliminacion(&raiz,id,band,&raiz);
                    if(validar==1)
                    {                          
                        pac.id=id;
                        strcpy(pac.nom," ");     
                        strcpy(pac.app," ");
                        strcpy(pac.apm," ");
                        pac.edad; 
                        pac.accion=2;                                                         
                        flujo =fopen("Pacientes.bin","ab");               
                        fwrite(&pac,sizeof(pac),1,flujo);                   
                        fclose(flujo);
                        printf("Paciente Eliminado\n");
                    }
                    else
                    {
                        printf("Paciente No Eliminado\n");
                    }

                    }
                    else{
                        printf("Arbol Vacio\n");
                    }
                    break;              
            }
    }while(opc!=5);
    return 1;
}