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

//DEFINIMOS EL VECTOR PARA LA POSICIÓN
typedef struct{
    double x, y;
} Vector;

//DEFINIMOS UN ARRAY PARA LA POSICIÓN DE TODAS LAS PARTÍCULAS
Vector particula[N];

double L;
double sigma=1.0; //diametro de la particula
double teta=0.05; //fraccion de area
double radio;


int main (){
    ini_ran(time(NULL));

    int i;
    double x,y;

    FILE *f;
    f=fopen("Posiciones_t_199_g_10.00.txt","r");

    for (i=0;(!feof(f))&&(i<N);i++){
        fscanf(f,"%lf   %lf ",&particula[i].x,&particula[i].y);

    }
    fclose(f);

    double Lx, Ly;

    L=sqrt((PI*(double)N*sigma)/(4*teta));
    Lx=0.4*L/sqrt(10);
    Ly=10*Lx;

    int intervalos;

    intervalos=(int)Ly+1;

    double *p_y;

    p_y=(double*)calloc(intervalos,sizeof(double));

    int j;

    for(i=0;i<N;i++){
        j=(int)particula[i].y;
        p_y[j]++;
    }
    FILE *f1;
    f1=fopen("Densidad.txt","w");

    for(i=0;i<intervalos;i++){
        fprintf(f1,"%d    %lf\n",i,p_y[i]);
    }

    fclose(f1);
    free(p_y);
}
