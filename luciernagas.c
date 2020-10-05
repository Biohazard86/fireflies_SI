// David Barrios Portales
// davidbarrios@usal.es
// LUCIERNAGAS 

/*
COMPILAR:

		gcc -o luciernagas luciernagas.c -lm  			(-lm es necesario por incluir la libreria math.h)

*/


/*  	Algoritmo:

		iniciar_poblacion la poblacion
		REPETIR
				Mover cada luciérnaga hacia las más brillantes 
				Actualizar el brillo de las luciérnagas
				Ordenarlas por brillo y encontrar la mejor
		HASTA (condición de parada)


 */




/*

	La intensidad de la luz que emite una luciernaga es una variable.

	La distancia influye en la fuente de luz y las caracteristicas del medio.

	PROBLEMA>>>

	Se consideran N luciernagas, cada una representa una solucion al problema, la calidad de esa solucion viene dada por el brillo asociado a cada luciernaga.

	Hay varias reglas:	
			- Todas las luciernagas son UNISEX: todas las lucierngas son atraidas por todas.
			-El atractivo de una luciernaga es proporcional a su brillo y disminuye con la distancia, por tanto una luciernaga se ve atraida hacia vecinos mas brillantes que ella
			mientras que la luciernaga mas brillante se mueve al azar
			- El brillo de una liciernaga se ve afectado por las caracteristicas de la funcion objetivo del problema.
	


	ALGORITMO AL DETALLE >>>>>>>>>>>>>>>>>>>>>>>

	A - Generacion de la poblacion inicial de luciernagas:
			Distribuir las N luciernagas aleatoriamente en el espacio de busqueda
			Calcular su brillo inicial.

	B - Mover cada luciernaga hacia las luciernagas mas brillantes que ella.
			- Cada luciernaga (Excepto la mas brillante) se mueve hacia las que son mas brillantes que ella.
			- Como resultado se calcula una nueva posiccion para cada luciernaga, en funcion de su posicion.
			- Cada luciernaga i se mueve hacia la luciernaga k, que es mas brillante que ella, aplicando la formula:  f(t+1)=fi(t)+B(rik)[fi(t)-fk(t)]+Ei
				DONDE:	fi(t+1) >> Nueva posicion de la luciernaga i
						fi(t)	>> posicion actual de la luciernaga i
						B(rik)  >> atractivo de la luciernaga k sobre i 
						Ei 		>> valor aleatorio
			- Atractivo de la luciernaga j sobre i:   B(rij)=B exp (-y rij ^ 2)
				DONDE: 	rij >> Distancia entre las luciernagas i y j
						B 	>> Atractivo a distancia cero
						y	>> Coeficiente de absorcion de la luz del medio.

	C - Mover la luciernaga mas brillante

			La luciernaga mas brillante no es atraida por ninguna otra, por lo que se mueve al azar

			Nueva posicion de la mejor luciernaga: fb (t+1)=fb(t) + E  DONDE E es un valor aleatorio.


	D - Actualizar intensidad de la luz

			Calcular el firness de cada luciernaga, en base a la funcion objetivo del problema.

	E - Ordenarlas por brillo y determinar la mejor.

			Tomar la luciernaga con el mejor brillo como mejor solucion.

*/






//  Librerias ---------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

// Constantes ----------------------------------------------------------------------------------------------

#define FUN_ESF 1
#define FUN_SUM_CUAD 2
#define MAXIMO 55555
#define MAX 10 //50 //Numero de iteracciones
#define N_PARTICULAS 5 // Numero de particulas P
#define R 2 // Dimension del espacio de solucion. Dimension del vector G

// ------------------------------------------------------------------------------------------------------------------------------------

// PROTOTIPOS
void iniciar_poblacion(double g[R], double x[N_PARTICULAS][R]);
double funcion_esfera(double vector[]);
double funcion_suma_cuadrados(double vector[]);
void actualizar_mas_brillante(double g[]);
void actualizar_posicion(double x[N_PARTICULAS][R] );
void actualizar_brillo( double x[N_PARTICULAS][R], double l[], int operacion);
void mostrar_sol(double g[R], int operacion);
int menu_inicio();
double operacion_seleccionada(double vector[], int operacion);
void bien_venido();
void mostrar_datos_particulas( double x[N_PARTICULAS][R]);
void buascar_mejor(double g[], double x[N_PARTICULAS][R], double l[], double *l_mejor);
void ordenacionSeleccionDirecta(double x[N_PARTICULAS][R], double b[]);

// ------------------------------------------------------------------------------------------------------------------------------------


int menu_inicio()  // Esta funcion muestra un menu el cual nos da a elegir entre minimizar usando la funcion esfera o minimizar usando la funcion sumacuadrado.
{
	int operacion; 
	printf("----------------------------- \n" );
	printf("         LUCIERNAGAS \n" );
	printf("----------------------------- \n\n" );
	printf("Que opcion quieres realizar? \n" );
	printf("1. Minimizar mediante la funcion esfera\n" );
	printf("2. Minimizar mediante la funcion suma cuadrados\n\n" );
	scanf("%d", &operacion); // leemos la opcion elegida
	return operacion; 
}


// ------------------------------------------------------------------------------------------------------------------------------------


// Funcion para buscar la mejor solucion
void buascar_mejor(double g[], double x[N_PARTICULAS][R], double l[], double *l_mejor)
{
	int i;

	if(l[0]<(*l_mejor))
	{
		for(i=0;i<R;i++)
		{
			g[i]=x[0][i];
		}
		(*l_mejor)=l[0];
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------
/*

Calcula el fitness de cada particula
    
    Parametros:
    - el vector de las posiciones de la particula que se va ha evaluar, vector[]
    - la variable que contiene el digito de la operación para calcular el fitness
    Retorna el fitness
*/

double operacion_seleccionada(double vector[], int operacion)
{
	double sol; // variable en la que almacenamos la solucion 

    // depende de la opcion elegida en el menu del inicio, se hara una u otra cosa.
	switch (operacion) {
        // Funcion esfera
		case FUN_ESF:
			sol=funcion_esfera(vector);
			return sol;

        //Funcion suma cuadrado
		case FUN_SUM_CUAD:
			sol=funcion_suma_cuadrados(vector);
			return sol;

        // si se introduce una opcion no valida
		default:
			printf("Opcion NO valida!\n");   // Se muestra el error en pantalla
			exit(1);	// Se sale del programa
	}
}


// ------------------------------------------------------------------------------------------------------------------------------------
/*
	Calcula el valor de la funcion esfera en funcion de un vector
	Parametros el vector, posicion de una particula
	Devuelve el valor de la funcion esfera para un vector
*/

double funcion_suma_cuadrados(double vector[])
{
	int i; 
	double suma=0; // variable que acumula el valor

    // aplico la formula de la funcion suma cuadrados
	for(i = 0; i<R; i++)
	{
		suma = ((vector[i]*vector[i])*(i+1))+suma; // sumatorio de los valores del vector, aplicando la formula de la funcion
	}

	return suma; // Valor del fitness ya calculado.
}


// ------------------------------------------------------------------------------------------------------------------------------------
/*
	Calcula el valor de la funcion esfera en funcion de un vector
	Los parametros son el vector y la posicion de una particula
	Devuelve el valor de la funcion esfera para un vector
*/

double funcion_esfera(double vector[])
{
	int i;
	double suma=0; // variable que acumula el valor

    // formula de la funcion esfera
	for(i = 0; i<R; i++)
	{
		suma = (vector[i]*vector[i])+suma; // sumatorio de los valores del vector aplicando la formula de la funcion
	}

	return suma; // Valor del fitness ya calculado.

}


// ------------------------------------------------------------------------------------------------------------------------------------
/*
	Funcion que calcula la mejor posición global, comparando la posicion guardada en G con las posiciones guardadas en B, si estas tienen un menor fitness se actualiza el vector G
    PARAMETROS:
    - g: vector que contiene la mejor solución encontrada en la iteraccion anterior
    - x: vector de las posiciones para cada particlua
    - operacion: variable que indica con que función se evalua el fitness de cada particula
*/
void actualizar_mas_brillante(double g[])
{
	int i,j;//contadores
	double mej_ant; // fitness de la particlua b a comparar
	double ale; //aleatorios

	for(i=0;i<R;i++)
	{
        /*   Calculo los aleatorios correspondientes   */
        ale=rand()%100;
        ale=ale/(ale+1);

		/*fbest(t+1) = fbest(t) + ε*/
		//mej_ant=g[i];
		g[i]=g[i]+ale;
	
	}
}

// ------------------------------------------------------------------------------------------------------------------------------------
/*
	Funcion que muestra la solucion que saca el algoritmo
	Parametros:
	- g: vector solución
    - operacion: variable que indica con que función se evalua el fitness de cada particula
	No devuelve nada (void)
*/

void mostrar_sol(double g[], int operacion)
{
	int i; // contador
	printf("El resultado es: \n");

	for(i=0;i<R;i++)
	{
		printf("%lf ", g[i]); // muestro las coordenadas de la solucion

	}
    printf("   ( Fitness = %lf )\n ", operacion_seleccionada(g, operacion)); //muestro el fitness de la solución

}


// ------------------------------------------------------------------------------------------------------------------------------------

//Con esta funcion generamos un num aleatorio.

double genero_rnd()
{
double ale;

        /*   Calculo los aleatorios correspondientes   */
        ale=rand()%100;
        ale=ale/(ale+1);
	 return ale;
}

//----------------------------------------------------------------------------------------------------------------------------------------

// distancia Euclidea entre 2 vectores
double calcular_distancia(double V1[], double V2[])
	{
	int i; // contador
	double dist =0; // distancia que se calculará

	for(i=0; i<R; i++){
		dist += pow(V1[i]-V2[i], 2);
	}
	//NOTA: como tengo que elevar el resultado al cuadrado
     // no calculo la raiz y me evito las dos operaciones
     // complementarias
	return dist;   //sqrt(dist);
}
// ------------------------------------------------------------------------------------------------------------------------------------
/*
	Funcion que actualiza la posicion y velocidad de cada particula
    PARAMETROS:
    - x: vector de las posiciones de la particula en la iteraccion actual
    - v: vector que guarda las velocidades de las particulas para cada coordenada
    - g: vector que contiene la mejor solución encontrada en la iteraccion anterior
    - b: vector de las mejores posiciones para cada particlua
*/

void actualizar_posicion(double x[N_PARTICULAS][R] )
{
	double ale; //aleatorios
	int i,j, k, m;//contador
	int y=1; // coeficiente absorción luz
	double Xant; // variables para guardar los datos de las particulas anteriores a la actualizacion
	double b;

	int b0=1;

double r_cuadrado; // valor usado para calcular el atractivo de una luci. sobre otra
double beta; //operación de la transparencia 11
double desplazamiento[R]; /* para ir acumulando valores de
                             desplazamiento de una luci hacia otras */

	/* para cada partícula,...
    hago un desplazamiento hacia las que son más brillantes
     que ellas */
	for(i=N_PARTICULAS-1; i>=0; i--)
    //OJO: DEBO RECORRER DESDE LA MENOS BRILLANTE HACIA LA MAS BRILLASNTE,
    // PARA APLICAR CORRECTAMENTE LOS VALORES DE (t) y (t+1)
    // QUE APARECEN EN LA PG. 10 (i=0;i<N_PARTICULAS;i++)
     {

 	   	// SI ES LA MAS BRILLANTE!!!!!
	   	if(i == 0)
		{
			// genero un aleatorio
			ale = genero_rnd();

			/* se lo sumo a todas las componentes del vector
               posición de la mejor luci. actual */
			for(j=0;j<R;j++)
				x[i][j] += ale;

 			for(j=0; j<R; j++)
			{
				if(x[i][j] < -5.12) x[i][j] = -5.12;
				else if(x[i][j] > 5.12) x[i][j] = 5.12;
			}

		}
	   	// SI NO ES LA MAS BRILLANTE!!!!!
		else
		{
			// genero un aleatorio
			ale = genero_rnd();

			/* pongo a cero todas las componentes del vector
  			desplazamiento, para ir luego sumando los desplazamientos
               hacia cada una de las luciernagas más brillantes que la i-ésima*/
			for(m=0; m<R; m++)
  				desplazamiento[m] = 0;


			// recorro las más brillantes que la i-ésima,
               // acumulando en el vector desplazamiento el segundo
               // sumando de la Ec. de la pg 10 que corresponde a
 			// cada luciérnaga
			for(k=0; k<i; k++)
			{
				/* distancia entre las posiciones de las
                    lucis. i y k (la actual y una que brilla mas)*/
				r_cuadrado = calcular_distancia(x[i], x[k]);
 				beta = b0 * exp(-y * r_cuadrado);


				// ahora calculo el segundo sumando de la
				// fórmula de la pg. 10, que determina cómo
                    // varía la posición de la luci. i al ser
                    // atraida hacia la posición de la luci. k (que
                    // es más brillante que ella
 				for(m=0; m<R; m++)
  					desplazamiento[m] += beta *
 						x[i][m]-x[k][m];
			}

			// ahora completo la aplicación de la fórmula de la pg.
               // 10, para actualizar la posición de la luci. i-esima
			for(m=0; m<R; m++)
				x[i][m] += desplazamiento[m] + ale ;

			//ajusto al intervalo [-5.12, 5.12]
			for(m=0; m<R; m++)
			{
				if(x[i][m] < -5.12) x[i][m] = -5.12;
				else if(x[i][m] > 5.12) x[i][m] = 5.12;
			}
		}
     }
}


// ------------------------------------------------------------------------------------------------------------------------------------
/*
    Funcion que inicia los valores de las posiciones de las particulas y los arrays de mejores posiciones con una posicion de las particulas
    PARAMETROS:
    - g: vector que contiene la mejor solución encontrada en la iteraccion anterior
    - b: vector de las mejores posiciones para cada particlua
    - x: vector de las posiciones de la particula en la iteraccion actual
    - v: vector que guarda las velocidades de las particulas para cada coordenada

*/

void iniciar_poblacion(double g[R], double x[N_PARTICULAS][R])
{
	int i,j; // contadores
	double ale; // número aleatiorio, paraa generar las posiciones

    /*   Inicio las posiciones de las particulas con números aleatorios   */
	for(i=0;i<N_PARTICULAS;i++)
	{
		for(j=0;j<R;++j)
		{
			ale = (1.0 + (rand()%100) )/ 100;
			x[i][j]= -5.12 +(5.12 - (-5.12)) * ale;

			//ale=rand()%100; // calculo el numero aleatorio de la coordenada
			//x[i][j]=ale; // lo almaceno

		}
	}

    /*   El vector de la mejor posicón lo inicio con los datos de la primera particula, si no es la verdadera solución el algoritmo lo cambara al pasar las iteracciones   */
	for(j=0;j<R;++j)
	{
		g[j]=x[0][j];
	}

    /*  muestro al usuario las posiciones iniciales de las particulas   */
    mostrar_datos_particulas(x);

}

// ------------------------------------------------------------------------------------------------------------------------------------
// FUncion para actualizar el brillo
void actualizar_brillo(double x[N_PARTICULAS][R], double l[], int operacion)
{
	int i;
	//(*l_mejor)=operacion_seleccionada(g, operacion);
	for(i=0;i<N_PARTICULAS;i++)
	{
		l[i]=operacion_seleccionada(x[i], operacion);
	}

}

// ------------------------------------------------------------------------------------------------------------------------------------
/*
    Función para mostrar los datos de posición de las particulas
     PARAMETROS:
     - x: vector de las posiciones de la particula en la iteraccion actual
*/
void mostrar_datos_particulas( double x[N_PARTICULAS][R])
{
    int i,j; // contador

    /*  muestro al usuario las posiciones iniciales de las particulas   */
    printf("  las particulas se inician en las posiciones:\n");

    for(i=0;i<N_PARTICULAS;i++)
    {
        printf("particula %d\t",i);
        for(j=0;j<R;++j)
        {

            printf("%lf ",x[i][j]);

        }
        printf("\n");
    }
    printf("\n");
}

// ------------------------------------------------------------------------------------------------------------------------------------
/*
	Funcion de Ordenacion directa
	PARAMETROS
	N: numero elementos del vector
	V(): vector a ordenar
*/
void ordenacionSeleccionDirecta(double x[N_PARTICULAS][R], double b[])
{
	int I, J, K ; // posiciones del vector
	double AUX; //variable auxiliar sobre la que se hacen los cambios de posicion

	/*
		bucle que intercambia los valores del vector de datos hasta que esten ordenados
		al final de la ejecucion de la pasada el contador I de posicion se aumenta en uno para ir ordeanado todos los vectores
	*/
	for(I = 0; I < (N_PARTICULAS); I++)
	{
		K = I; //contador de donde esta el dato mas pequeño, empiezo dandole el valor de la primera posicion de la pasada
		AUX = b[I]; //guardo el valor de la primera posicion de la pasada y compruebo si es el más pequeño

		/*
			bucle que recorre todo el vector de datos
			cuando encuentra un valor menor al almacenado en la variable AUX se establece como nuevo valor de esa variable
		*/
		for( J = (I +1) ; J < N_PARTICULAS ; J++ )
		{
			if(b[J] < AUX) //verifico si hay numeros menores al valor de la variable AUX
			{
				K = J; //guardo la posicion del vector donde esta el valor mas pequeño encontrado
				AUX = b[J]; //cambio el numero menor al encontrado como nuevo menor
			}
		}

		b[K] = b[I]; // guardo el valor desordenado en la posicio del meor encontrado en la pasada
		b[I] = AUX; // el valor menor encontrado en la pasada se almacena en la pimera posicion no ordeada del vactor

        for(J=0;J<R;J++)
        {
			AUX=x[I][J];
			x[I][J]=x[K][J];
			x[K][J]=AUX;
        }
	}
}



// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------------------------------------------------


int main()  //Funcion main.
{
    srand(time(NULL)); // semilla para los números aleatorios y que cada vez se generen unos diferentes.
	//mejor solucion del enjambre g(t) en el algoritmo
	double g[R]; //vector que almacena la mejor posición encontrada por el algoritmo, de entre todas las particulas
	//vectores posiciones y velocidad
	double x[N_PARTICULAS][R]; //pos actual, x[0] posición de la primera partícula, hasta X[P-1]
	double l[N_PARTICULAS]; // brillo de las luciernagas
	double l_mejor = MAXIMO; // mejor brillo

    int j,k,i ; // contadores

    int operacion=0;

    operacion = menu_inicio(); // solicito al uruario con que función se va ha ejecutar


//|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||

	// OPERACION A DE LOS APUNTES

     //A-1 posicion inicial
	iniciar_poblacion(g,x); // inicio las particulas y los demás vectores


    //A-2 calculo el brillo incial de las luciérnagas
    actualizar_brillo(x,l, operacion);

	

	printf("\nBrillo inicial:  \n");
		for(j=0; j<N_PARTICULAS; j++)
			printf(" %.3lf \n", l[j]);
          	

    //Ordenarlas por brillo y encontrar la mejor
    ordenacionSeleccionDirecta(x,l);

    printf("\n\nBrillo despues de ordenar:  \n");
    for(j=0; j<N_PARTICULAS; j++)
		printf("     %.3lf\n", l[j]);
    

	printf("\n\n");
 	// encontrar mejor inicial
 	buascar_mejor(g,x,l,&l_mejor);


 	printf("INICIALMENTE la mejor solución es: \n");
     mostrar_sol(g, operacion);


	//Repetir, tantas veces como mande la iteraccion, marcado en una constante MAX
    printf("ITERACCIONES DEL ALGORITMO: \n");// indico al usuario el comienzo de la ejecución

    /*   Bucle que se ejecuta tantas veces como la constante MAX indique,
    cuantas más iteracciones más exacta sera la solucion que conseguira el algoritmo    */
	for(i=0;i<MAX; i++)
	{
        // muestro informacion sobre la posicion de las particulas en la iteracción
         printf("\n --->  Iteraccion %d\n",i); // informo al usuario de la iteraccion actual
         printf("posiciones de las particulas en la iteraccion:\n"); // muestro las posiciones de las particulas y su fitness
        //MARISA ANULO: mostrar_datos_particulas(x);


		 //Actualizar el brillo de las luciérnagas
		// actualizar_brillo(g,x,l, &l_mejor,operacion);


		 // encontrar mejor
		//QUITO buascar_mejor(g,x,l,&l_mejor);

        /*  El fitness de cada particulas se calcula cuando se evalua la posicion actual con la de mejor de la paricula   */
        //Mover cada luciérnaga hacia las más brillantes Ec.1 Mover la luciérnaga más brillante Ec.2
        actualizar_posicion(x);

	     //Actualizar el brillo de las luciérnagas
	     actualizar_brillo(x,l, operacion);

		//Ordenarlas por brillo
	 	ordenacionSeleccionDirecta(x,l);

		 printf("\nBrillo despues de ordenar:  \n");
		for(j=0; j<N_PARTICULAS; j++)
			printf(" %.3lf", l[j]);
          

          /* encontrar mejor
           (DE EXISTIR UNA  printf("en la iteraccion %d, la mejor solución es: \n", i); // informo de la mejor solucion de le iteracción
        mostrar_sol(g, operacion);LUCI. MEJOR EN LA POBLACIÓN ACTUAL,
            SERÍA LA PRIMERA EN LOS DATOS ORDENADOS) */
		buascar_mejor(g,x,l,&l_mejor);

//		actualizar_mas_brillante(x, g);
        //Actualizar el brillo de las luciérnagas

        printf("\n");

        printf("en la iteraccion %d, la mejor solución es: \n", i); // informo de la mejor solucion de le iteracción
        mostrar_sol(g, operacion);


		printf("\n\n\n ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||| \n\n");
	}

    // UNA VEZ ACABAN LAS ITERACCIONES DEL ALGORITMO

	//muestro la mejor solucion g(t)
	printf(" \n\tSOLUCION FINAL: \n\n\t"); // mensaje de fin de ejecución
	mostrar_sol(g, operacion); // llamo a la función que muestra los valores de array indicado
    printf("\n\n");
	return 0;
}
