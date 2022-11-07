#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// ALGORITMO P-R
extern float Random(void);
#define NormRANu (2.3283063671E-10)
extern void ini_ran(int SEMILLA);
unsigned int irr[256];
unsigned int ir1;
unsigned char ind_ran, ig1, ig2, ig3;

#define N 1000
#define PI 3.14159265

// GENERACION DE NUMEROS
float Random(void)	// generar numero aleatorio
{
    float r;
    ig1=ind_ran-24;
    ig2=ind_ran-55;
    ig3=ind_ran-61;
    irr[ind_ran]=irr[ig1]+irr[ig2];
    ir1=(irr[ind_ran]^irr[ig3]);
    ind_ran++;
    r=ir1*NormRANu;
    return r;
}

void ini_ran(int SEMILLA)
{
    int INI,FACTOR,SUM,i;
    srand(SEMILLA);
    INI=SEMILLA;
    FACTOR=67397;
    SUM=7364893;
    for(i=0; i<256; i++)
    {
        INI=(INI*FACTOR+SUM);
        irr[i]=INI;
    }
    ind_ran=ig1=ig2=ig3=0;
}

double modulo(double x, double y){
    double d;
    d=sqrt(x*x+y*y);
    return d;
}
//DEFINIMOS EL VECTOR PARA LA POSICIÓN
typedef struct{
    double x, y;
} Vector;

//DEFINIMOS UN ARRAY PARA LA POSICIÓN DE TODAS LAS PARTÍCULAS
Vector particula[N];
Vector particula_c[N];//posicion considerando la caja

//POSICIÓN DE LAS PARTICULAS EN EL TIEMPO ANTERIOR
Vector particula0[N];
Vector particula0_c[N];//posicion anterior considerando la caja

//POSICION DE LAS PARTICULAS EN LA CONFIGURACION INICIAL
Vector posicion0[N];


double L;
double sigma=1.0; //diametro de la particula
double teta=0.05; //fraccion de area
double radio;

double d;//distancia entre los centros
double d_x;//diferencia entre la coordenada x de dos particulas
double d_y;



int main (){
    ini_ran(time(NULL));
    radio=sigma/2;

    int L_red; //longitud del lado de la red
    double a;// lo que hay que sumar a la componente y al cambiar de fila
    double x0, y0; //posicion de la primera particula en la red


    double T=100000;//pasos de tiempo

    int t;//indice del tiempo
    int h;

    int i,j,n;

    double MSD;

    double e_x, e_y;//número random
    double delta;
    delta=sigma/10;

    double x_aux, y_aux, d_aux;

    int k;

    double Lx, Ly;

    L=sqrt((PI*(double)N*sigma)/(4*teta));
    Lx=0.4*L/sqrt(10);
    Ly=10*Lx;
    //Lx=L;
    //Ly=L;

    //printf("%lf\n%lf",Lx,Ly);

    //DECLARAMOS VARIABLES PARA LA GRAVEDAD, LA ENERGÍA, LA MASA, LA TEMPERATURA
    double g;
    double E; double E_0; double delta_E;
    double m;
    double Temp;
    double kB;
    double random;

    g=1; m=1.0; Temp=1.0; kB=1.0;

    //DEFINIMOS UNA VARIABLE PARA LA PROBABILIDAD
    double Prob;


    //INICIALIZAMOS LA POSICION DE LAS PARTICULAS ALEATORIAMENTE

    /*FILE*f;
    f=fopen("Conf_inicial.txt","w");*/
    //INICIALIZAMOS LA POSICIÓN DE LA PRIMERA PARTICULA
    for(k=1;k<2;k++){
        switch (k){
        case 0:
            g=10;
            break;
        case 1:
            g=1;
            break;
        case 2:
            g=0.1;
            break;
        case 3:
            g=0.01;
            break;
        case 4:
            g=0;
            break;

        }

        particula[0].x=(sigma/2)+(Lx-sigma)*Random();
        particula[0].y=(sigma/2)+(Ly-sigma)*Random();
        posicion0[0]=particula[0];


        for (i=1;i<N;i++){

        particula[i].x=(sigma/2)+(Lx-sigma)*Random();
        particula[i].y=(sigma/2)+(Ly-sigma)*Random();

            for(j=0;j<i;j++){
                    d_x=particula[i].x-particula[j].x;
                    d_y=particula[i].y-particula[j].y;
                    d=modulo(d_x,d_y);

                   if(d<sigma){
                      particula[i].x=(sigma/2)+(Lx-sigma)*Random();
                      particula[i].y=(sigma/2)+(Ly-sigma)*Random();
                      j=-1;
                   }

            }
            posicion0[i]=particula[i];
              //fprintf(f,"%lf      %lf         %lf\n",particula[i].x,particula[i].y,radio);


        }
        //fclose(f);

        h=0;
        //EVOLUCIONAMOS LA POSICION DE LAS PARTICULAS
        for(t=0;t<T;t++){
            //FICHERO PARA ESCRIBIR LAS POSICIONES DE LAS PARTICULAS EN UN TIEMPO

            FILE*f1;
            if((t%500)==0){
                //FICHERO PARA ESCRIBIR LAS POSICIONES DE LAS PARTICULAS EN UN TIEMPO
                char filename[40];
                sprintf(filename, "Posiciones_t_%d_g_%.2lf.txt", h,g);
                f1=fopen(filename, "w");
                h++;
            }

            for(i=0;i<N;i++){

                particula0[i]=particula[i];


                e_x=Random();
                e_y=Random();
                //printf("%lf   %lf\n",e_x,e_y);



                particula[i].x=particula0[i].x+delta*(e_x-0.5);
                particula[i].y=particula0[i].y+delta*(e_y-0.5);
                //printf("%lf   %lf\n",particula0[i].y,particula[i].y);


                n=0;
                //PRIMERO HAY QUE COMPROBAR SI ACEPTAMOS EL PASO CON LA ENERGIA

                E=m*g*particula[i].y;
                E_0=m*g*particula0[i].y;

                delta_E=E-E_0;
                //printf("%lf   ",delta_E);


                Prob=exp(-(delta_E)/(kB*Temp));
                //printf("%lf\n",Prob);

                if(Prob<1.0){
                    random=Random();
                    if(random>Prob){
                        //printf("%d\n",i);
                         particula[i]=particula0[i];
                         n=1;
                    }
                }

                //Ahora hay que comprobar si se solapa con alguna partícula
                //o si choca con la pared

                //Comprobamos si choca con alguna pared
                if(((particula[i].x+(sigma/2))>Lx)&&(n==0)){
                    particula[i]=particula0[i];
                    n=1;
                }
                if(((particula[i].x-(sigma/2))<0)&&(n==0)){
                    particula[i]=particula0[i];
                    n=1;
                }
                if(((particula[i].y+(sigma/2))>Ly)&&(n==0)){
                    particula[i]=particula0[i];
                    n=1;
                }
                if(((particula[i].y-(sigma/2))<0)&&(n==0)){
                    particula[i]=particula0[i];
                    n=1;
                }

                //Ahora hay que comprobar si se solapa con alguna partícula
                for(j=0;((j<N)&&(n==0));j++){
                    //printf("%d\n",i);
                    d_x=particula[i].x-particula[j].x;
                    d_y=particula[i].y-particula[j].y;
                    d=modulo(d_x,d_y);

                    if((d<sigma)&&(j!=i)){ //(d>(L-sigma))||

                        particula[i]=particula0[i];
                        j=N;
                    }


                }
                if((t%500)==0){
                    fprintf(f1," %lf     %lf\n",particula[i].x,particula[i].y);
                }

            }
            if((t%500)==0){
                fclose(f1);
            }
        }
    }




    return 0;
}
