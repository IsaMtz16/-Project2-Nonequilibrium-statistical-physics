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





int main (){
    ini_ran(time(NULL));
    radio=sigma/2;
    L=sqrt((PI*(double)N*sigma)/(4*teta));



    int i,j,k;
    int n,h;


    double e_x, e_y;//número random
    double delta;
    delta=sigma/10;

    int T=100;//Número de pasos de tiempo
    int t;//índice para llevar la cuenta del número de pasos

    double MSD;
    double D;//Diffusivity

    double x_aux, y_aux, d_aux;


    double d;//distancia entre los centros
    double d_x;//diferencia entre la coordenada x de dos particulas
    double d_y;

    double disp_x;//diferencia entre la coordenada x de dos particulas
    double disp_y;

    //FICHERO PARA ESCRIBIR D EN FUNCION DE DELTA
    FILE*f3;
    f3=fopen("D_delta.txt","w");

    //printf("%lf",L);
    //BUCLE PARA DIFERENTES VALORES DE DELTA
    for(k=0;k<6;k++){
        switch (k){
        case 0:
            delta=0.001*sigma;
            break;
        case 1:
            delta=0.003*sigma;
            break;
        case 2:
            delta=0.01*sigma;
            break;
        case 3:
            delta=0.03*sigma;
            break;
        case 4:
            delta=0.1*sigma;
            break;
        case 5:
            delta=0.3*sigma;
            break;
        }


         //INICIALIZAMOS ALEATORIAMENTE LA POSICION DE LAS PARTICULAS
        /*FILE*f;
        f=fopen("Conf_inicial.txt","w");*/
        //INICIALIZAMOS LA POSICIÓN DE LA PRIMERA PARTICULA
        particula[0].x=(sigma/2)+(L-sigma)*Random();
        particula[0].y=(sigma/2)+(L-sigma)*Random();
        particula_c[0]=particula[0];
        posicion0[0]=particula[0];

        for (i=1;i<N;i++){

        particula[i].x=(sigma/2)+(L-sigma)*Random();
        particula[i].y=(sigma/2)+(L-sigma)*Random();

            for(j=0;j<i;j++){
                d_x=particula[i].x-particula[j].x;
                d_y=particula[i].y-particula[j].y;
                d=modulo(d_x,d_y);

               if(d<sigma){
                  particula[i].x=(sigma/2)+(L-sigma)*Random();
                  particula[i].y=(sigma/2)+(L-sigma)*Random();
                  j=-1;
               }

        }
        posicion0[i]=particula[i];
        particula_c[i]=particula[i];
        //fprintf(f,"%lf      %lf         %lf\n",particula[i].x,particula[i].y,radio);

        }
        //fclose(f);



        //FICHERO PARA ESCRIBIR MSD vs t PARA DIFERENTES VALORES DE DELTA
        FILE*f2;
        char filename2[40];
        sprintf(filename2, "MSD(t)_delta_%.3lf.txt", delta);
        f2=fopen(filename2, "w");



        //VAMOS A EVOLUCIONAR  LA POSICIÓN DE LAS PARTICULAS




        for(t=0;t<T;t++){



             MSD=0;

            //FICHERO PARA ESCRIBIR LAS POSICIONES DE LAS PARTICULAS EN UN TIEMPO
            /*FILE*f1;
            char filename[40];
            sprintf(filename, "Posiciones_t_%d_delta_%.3lf.txt", t,delta);
            f1=fopen(filename, "w");*/

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
                    if((particula[i].y+(2*sigma))>=L){
                        y_aux=particula[i].y-L;
                        n=1;
                    }
                }




                //Ahora hay que comprobar si se solapa con alguna partícula
                for(j=0;j<N;j++){

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
                    //printf("%d\n",i);
                    //Comprobamos el caso en el que la particula esté en el borde de la caja







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






                //fprintf(f1," %lf     %lf\n",particula[i].x,particula[i].y);


                //printf("%lf   %lf\n",d_x,d_y);
                //fprintf(f5," %d    %lf\n",t,MSD_0);
                //displacement[i]=displacement[i]+sqrt(d_x*d_x+d_y*d_y);
                //MSD=MSD+(displacement[i]*displacement[i])/((double)t);

                disp_x=particula_c[i].x-posicion0[i].x;
                disp_y=particula_c[i].y-posicion0[i].y;
                MSD=MSD+pow(disp_x,2)+pow(disp_y,2);

                //printf("%lf\n",sqrt(pow(disp_x,2)+pow(disp_y,2)));

            }



            MSD=MSD/(double)N;


            fprintf(f2,"%d   %lf\n",t,MSD);



            //fclose(f1);
        }
        fclose(f2);
        D=MSD/(4*(double)T);
        //printf("%lf    %lf\n",MSD_0,D);
        fprintf(f3,"%lf     %lf\n",delta,D);

    }
    fclose(f3);

    return 0;
}
