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


    double T=15000;//pasos de tiempo

    int t;
    int g;

    int i,j,n;

    double MSD;

    double e_x, e_y;//número random
    double delta;

    double x_aux, y_aux, d_aux;

    int k;
    //BUCLE PARA DIFERENTES VALORES DE TETA
    for(k=0;k<2;k++){

        switch (k){
        case 0:
            teta=0.05;
            break;
        case 1:
            teta=0.2;
            break;
        case 2:
            teta=0.5;
            break;

        }


        L=sqrt((PI*(double)N*sigma)/(4*teta));
        //printf("%lf\n",L);
        /*FILE*f;
        f=fopen("Conf_inicial.txt","w");*/

        //INICIALIZAMOS LA POSICION DE LAS PARTICULAS EN UNA RED TRIANGULAR
        //CENTRADA EN EL CENTRO DE LA CAJA

        //En una misma fila la distancia entre centros es sigma
        //Para cambiar de fila hay que añadir sigma/2 en x y sigma*sqrt(3)/2 en y



        L_red=(int)sqrt(N)+1;


        a=sqrt(3)*sigma/2;

        x0=(L-L_red)/2;
        y0=(L-L_red)/2;



        n=0;
        for(i=0;(i<L_red)&&(n<N);i++){
            if((i%2)!=0){
                particula[n].x=x0+sigma/2;
            }
            else{
                particula[n].x=x0;
            }

            particula[n].y=y0+i*a;
            posicion0[n]=particula[n];
            particula_c[n]=particula[n];
            //fprintf(f,"%lf      %lf\n",particula[n].x,particula[n].y);
            n++;
            for(j=1;(j<L_red)&&(n<N);j++){
                particula[n].x=particula[n-1].x+sigma;
                particula[n].y=particula[n-1].y;
                //fprintf(f,"%lf      %lf\n",particula[n].x,particula[n].y);
                posicion0[n]=particula[n];
                particula_c[n]=particula[n];
                n++;
            }

        }
    //fclose(f);

        //AHORA EVOLUCIONAMOS EL SISTEMA




        delta=sigma/10;


        g=0;
        //FICHERO PARA ESCRIBIR MSD vs t PARA DIFERENTES VALORES DE TETA
        FILE*f2;
        char filename2[40];
        sprintf(filename2, "MSD(t)_teta_%.2lf.txt", teta);
        f2=fopen(filename2, "w");



        for(t=0;t<T;t++){
                MSD=0;
                //printf("%d\n",t);
                 FILE*f1;
                if((t%150)==0){
                    //FICHERO PARA ESCRIBIR LAS POSICIONES DE LAS PARTICULAS EN UN TIEMPO

                    char filename[40];
                    sprintf(filename, "Posiciones_t_%d_teta_%.2lf.txt", g,teta);
                    f1=fopen(filename, "w");
                    g++;
                }



                for(i=0;i<N;i++){


                    particula0[i]=particula[i];
                    particula0_c[i]=particula_c[i];

                    e_x=Random();
                    e_y=Random();

                    particula[i].x=particula0[i].x+delta*(e_x-0.5);
                    particula[i].y=particula0[i].y+delta*(e_y-0.5);

                    particula_c[i].x=particula0_c[i].x+delta*(e_x-0.5);
                    particula_c[i].y=particula0_c[i].y+delta*(e_y-0.5);

                    n=0;
                    x_aux=particula[i].x;
                    y_aux=particula[i].y;
                    d_aux=sigma;


                    if((particula[i].x-(sigma))<=0){
                        x_aux=particula[i].x+L;
                        n=1;
                    }
                    else{
                        if((particula[i].x+(sigma))>=L){
                            x_aux=particula[i].x-L;
                            n=1;
                        }
                    }

                    if((particula[i].y-(sigma))<=0){
                        y_aux=particula[i].y+L;
                        n=1;
                    }
                    else{
                        if((particula[i].y+(sigma))>=L){
                            y_aux=particula[i].y-L;
                            n=1;
                        }
                    }


                    //Ahora hay que comprobar si se solapa con alguna partícula
                    for(j=0;j<N;j++){
                        //printf("%d\n",i);


                        if(n==1){
                            d_aux=modulo((particula[j].x-x_aux),(particula[j].y-y_aux));
                        }

                        if(n==1){
                           if((d_aux<sigma)&&(j!=i)){ //(d>(L-sigma))||

                            particula[i]=particula0[i];
                            particula_c[i]=particula0_c[i];

                            j=N;
                            n=-1;
                            }
                        }


                       if(n!=-1){

                        d_x=particula[i].x-particula[j].x;
                        d_y=particula[i].y-particula[j].y;
                        d=modulo(d_x,d_y);

                           if((d<sigma)&&(j!=i)){ //(d>(L-sigma))||

                                particula[i]=particula0[i];
                                particula_c[i]=particula0_c[i];

                                j=N;
                                n=-1;
                            }
                        }



                    }

                    if(n!=-1){
                        //Comprobamos si sale de la caja
                        if((particula[i].x)>L){
                            particula[i].x=particula[i].x-L;
                        }
                        if((particula[i].x)<0){
                            particula[i].x=particula[i].x+L;
                        }
                        if((particula[i].y)>L){
                            particula[i].y=particula[i].y-L;
                        }
                        if((particula[i].y)<0){
                            particula[i].y=particula[i].y+L;
                        }

                    }


                    if((t%150)==0){
                       fprintf(f1," %lf     %lf\n",particula[i].x,particula[i].y);
                    }



                    //CALCULAMOS MSD (MEAN SQUARE DISPLACEMENT)
                    d_x=particula_c[i].x-posicion0[i].x;
                    d_y=particula_c[i].y-posicion0[i].y;
                    MSD=MSD+pow(d_x,2)+pow(d_y,2);



                }

                MSD=MSD/(double)N;

                if((t%150)==0){
                    fprintf(f2,"%d     %lf\n",t,MSD);
                    fclose(f1);
                }

        }
    fclose(f2);
    }





return 0;
}
