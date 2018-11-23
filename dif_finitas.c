#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*----- colores texto -----*/
#define NRM  "\x1B[0m"
#define RED  "\x1B[31m"
#define GRN  "\x1B[32m"
#define YEL  "\x1B[33m"
#define BLU  "\x1B[34m"
#define MAG  "\x1B[35m"
#define CYN  "\x1B[36m"
#define WHT  "\x1B[37m"
// cambiar color
// printf("%s", WHT);
/*----- colores texto -----*/

#define BACKWARD -1   // hacia atras
#define CENTERED  0   // centradas
#define FORWARD   1   // hacia adelante

FILE *file_graf;
FILE *file_error;

int intervalos = 100;
float extrA    = 0.0 * M_PI;
float extrB    = 2.0 * M_PI;

float delta;
float error_acm;

// -------------------- funciones -------------------- //
float Funcion(float x){
	// retornar valor de funcion a derivar
	return sin(x);
}

float Derivada(float x){
	// retornar valor de derivada analitica
	return cos(x);
}

float DiferenciasFinitas(float x, float dlt, int dir){
	float y = 0;
	switch(dir){
		case -1:
		y = ( Funcion(x) - Funcion(x - dlt) ) / dlt;
		break;
		case  0:
		y = ( Funcion(x + dlt) - Funcion(x - dlt) ) / (2.0 * dlt);
		break;
		case  1:
		y = ( Funcion(x + dlt) - Funcion(x) ) / dlt;
		break;
		default:
		y = 0;
		break;
	}
	return y;
}
// -------------------- funciones -------------------- //

int main(int argc, char const *argv[]){
	int i;
	float _x_, _y_, sum_y;
	delta = (extrB - extrA) / intervalos;
	
	printf("%s", GRN);
	printf("\nMETODO DE DIFERENCIAS FINITAS\n");
	printf("FUNCION: Y = SEN(X)\n");
	printf("%s", NRM);

	printf("\nINTERVALOS: %d\n", intervalos);
	printf("EXTREMO A:  %f\n", extrA);
	printf("EXTREMO B:  %f\n", extrB);
	printf("DELTA:      %f\n", delta);

	file_graf = fopen("grafica.dat","w");
	file_error = fopen("error.dat","w");

	fprintf(file_graf, "set xlabel \"X\"\n");
	fprintf(file_graf, "set ylabel \"Y\"\n");
	fprintf(file_graf, "set xrange [%f:%f]\n", extrA, extrB);
	fprintf(file_graf, "set yrange [-2:2]\n\n");

	fprintf(file_error, "   PUNTO - ANALITICO - DIF.FIN - ERROR\n\n");

	_x_ = extrA;
	_y_ = DiferenciasFinitas(_x_, delta, FORWARD);
	error_acm = Derivada(_x_) - _y_;
	fprintf(file_error, "%f  %f  %f  %f\n", _x_, Derivada(_x_), _y_, (Derivada(_x_) - _y_));
	sum_y = _y_;
	fprintf(file_graf,"set arrow %d from %f,0 to %f,%f lw 2 lc rgb \"red\"\n", 1, _x_, _x_, _y_);

	for(i=1; i<intervalos; i++){
		_x_ = _x_ + delta;
		_y_ = DiferenciasFinitas(_x_, delta, CENTERED);
		error_acm = Derivada(_x_) - _y_;
		fprintf(file_error, "%f  %f  %f  %f\n", _x_, Derivada(_x_), _y_, (Derivada(_x_) - _y_));
		sum_y = sum_y + _y_;
		fprintf(file_graf,"set arrow %d from %f,0 to %f,%f lw 2 lc rgb \"red\"\n", i+1, _x_, _x_, _y_);
	}

	_x_ = _x_ + delta;
	_y_ = DiferenciasFinitas(_x_, delta, BACKWARD);
	error_acm = Derivada(_x_) - _y_;
	fprintf(file_error, "%f  %f  %f  %f\n", _x_, Derivada(_x_), _y_, (Derivada(_x_) - _y_));
	sum_y = sum_y + _y_;
	fprintf(file_graf,"set arrow %d from %f,0 to %f,%f lw 2 lc rgb \"red\"\n", i+1, _x_, _x_, _y_);

	fprintf(file_graf, "plot sin(x) lw 2\n");
	fprintf(file_graf, "\npause -1\n");
	
	printf("%s", YEL);
	printf("\nSUMA(Y):    %f\n", sum_y);
	printf("ERROR ACM:  %f\n", error_acm);
	printf("ERROR PROM: %f\n", error_acm/(i+1));
	printf("%s", NRM);

	fprintf(file_error, "\n\nERROR ACM:  %f\n", error_acm);
	fprintf(file_error, "ERROR PROM: %f\n", error_acm/(i+1));

	fclose(file_graf);
	fclose(file_error);

	system("gnuplot \"grafica.dat\"");
	
	return 0;
}

