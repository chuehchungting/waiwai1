#include "stdafx.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<math.h>

FILE* fp, * fp16, * SP; //此FILE結構fp, fp16, SP, 可用於處理文件流以讀取、寫入或操作文件。

clock_t start1;

double now_time;

char parafile[20] = "param.txt", line[450], buf[11];

int N_Machine, N_Vehicle, N_Order, init_solu_method, N_Particle, iteration, vehicle, tmp_vehicle_size, * order_size, vehicle_size, check = 0, chck = 0;
int* M_select, * Order1_seq, * V_select, * Order2_seq, * tmp_vehicle_size1, * tmp_vehicle_cap, ** tmp_vehicle_order, * tmp_vehicle, * mach_previousjob, * order_finishtime_orderno;
int samea, * count_m, * count_v, ** mach_order_seq, ** vehicle_order_seq, tcount = 0;
int m_w_para, order_para, shak_iter;

float end_time, * settime0, ** settime1, * revenue, * proctime, * traveltime0, ** traveltime1, * duedate0, * duedate1, * weight, * setcost0, ** setcost1;
float* M_selectval, * Order1_seqval, * V_selectval, * Order2_seqval, pso_w, * pso_c1, pso_c2, * M_velocity, * V_velocity, * Order1_velocity, * Order2_velocity;
float* mach_usage_time, * order_finishtime, * avgproctime, * avgtraveltime, * avgtime, * t2_0;
float best_obj, ** mach_order_time, ** vehicle_order_time, * t1_tmp, * t2_tmp, shaking_ratio, curr_obj,cr;

float RandomF()
{
	float x;  x = (float)1.0 / (float)(RAND_MAX) * (float)rand();  return (x);
}

float RandomRge(float LB, float UB)
{
	float x;       x = LB + (UB - LB) / (RAND_MAX) * (float)rand();  return (x);
}

struct bettersolu {
	int* M_select;   int* V_select; int* Order1_seq;  int* Order2_seq;  float obj; float* Order1_seqval; float* Order2_seqval; float* M_selectval;
	float* V_selectval; float* M_velocity; float* V_velocity; float* Order1_velocity; float* Order2_velocity; //int particle_index;
};
struct bettersolu Curr, Curr1, Curr2, Best1, * PSO, * PB, GB, Test,DEV,DEU;

double timing(clock_t start_t) { double time;   clock_t cur_t;  cur_t = clock();   time = (double)(cur_t - start_t) / CLOCKS_PER_SEC;   return time; }

void GenData()
{
	FILE* fp1, * fp2, * fp3, * fp4, * fp6, * fp7, * fp8, * fp9, * fp10, * fp11, * fp12, * fp13, * fp14, * fp15;

	int seed, count_dis, size, problem_no;
	float settime0_lb, settime0_ub, settime1_lb, settime1_ub, proctime_lb, proctime_ub, traveltime0_lb, traveltime0_ub, traveltime1_lb, traveltime1_ub, weight_lb, weight_ub;
	float setcost0_lb, setcost0_ub, setcost1_lb, setcost1_ub, duedate0_lb, duedate0_ub, duedate1_lb, duedate1_ub, revenue_lb, revenue_ub, x_range, y_range, cap_lb, cap_ub;
	float settime, setcost, distance_facility, ** distance, x_facility, y_facility, * x, * y, tmpvv, pso_c1_init;

	fp = fopen("param.txt", "r");   fgets(line, 450, fp);

	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i - 1];       N_Machine = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 9];       N_Vehicle = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 19];      N_Order = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 29];      setcost0_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 39];      setcost0_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 49];      setcost1_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 59];      setcost1_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 69];      settime0_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 79];      settime0_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 89];      settime1_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 99];      settime1_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 109];      proctime_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 119];      proctime_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++) 	  buf[i - 1] = line[i + 129];      traveltime0_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 139];     traveltime0_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 149];     traveltime1_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 159];     traveltime1_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 169];     duedate0_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 179];     duedate0_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 189];     duedate1_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 199];     duedate1_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 209];     cap_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 219];     cap_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 229];     weight_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 239];     weight_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 249];     revenue_lb = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 259];     revenue_ub = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 269];     size = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 279];     seed = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 289];     x_range = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 299];     y_range = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 309];     problem_no = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 319];     end_time = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 329];     shaking_ratio = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 339];     init_solu_method = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 349];     N_Particle = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 359];     pso_w = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 369];     pso_c1_init = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 379];     pso_c2 = (float)atof(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 389];     shak_iter = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 399];     m_w_para = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 409];     order_para = atoi(buf);
	for (int i = 1; i <= 10; i++)     buf[i - 1] = line[i + 419];     cr = (float)atof(buf);

	settime0 = new float[N_Order];   revenue = new float[N_Order];      proctime = new float[N_Order];  traveltime0 = new float[N_Order];
	duedate0 = new float[N_Order];   duedate1 = new float[N_Order];     weight = new float[N_Order];     setcost0 = new float[N_Order];     order_size = new int[N_Order];
	x = new float[N_Order];          y = new float[N_Order];            pso_c1 = new float[N_Particle];

	distance = new float* [N_Order];
	for (int i = 1; i <= N_Order; i++)
		distance[i - 1] = new float[N_Order];

	for (int i = 1; i <= N_Particle; i++)
		pso_c1[i - 1] = pso_c1_init;

	fp1 = fopen("location.txt", "w");  fp2 = fopen("distance.txt", "w");   fp3 = fopen("weight.txt", "w");
	fp4 = fopen("revenue.txt", "w");

	fp6 = fopen("cap.txt", "w");
	fp7 = fopen("tw_0.txt", "w");
	fp8 = fopen("tw_1.txt", "w");
	fp9 = fopen("settime_0.txt", "w");
	fp10 = fopen("settime_1.txt", "w");
	fp11 = fopen("processtime.txt", "w");
	fp12 = fopen("setcost_0.txt", "w");
	fp13 = fopen("setcost_1.txt", "w");
	fp14 = fopen("traveltime_0.txt", "w");
	fp15 = fopen("traveltime_1.txt", "w");

	srand(seed);

	x_facility = RandomF() * x_range;
	y_facility = RandomF() * y_range;

	for (int i = 1; i <= N_Order; i++)
	{
		x[i - 1] = RandomF() * x_range;
		y[i - 1] = RandomF() * y_range;
		fprintf(fp1, "%10.2f %10.2f\n", x[i - 1], y[i - 1]);

		distance_facility = (float)hypot(x[i - 1] - x_facility, y[i - 1] - y_facility);
		if (distance_facility == 0)
			distance_facility = (float)0.01;
		fprintf(fp14, "%10.2f\n", 1.2 * distance_facility);
	}
	for (int i = 1; i <= N_Order; i++)
	{
		for (int j = 1; j <= N_Order; j++)
		{
			distance[i - 1][j - 1] = (float)hypot(x[i - 1] - x[j - 1], y[i - 1] - y[j - 1]);
			if ((distance[i - 1][j - 1] == 0) && (i != j))
				distance[i - 1][j - 1] = (float)0.01;
			fprintf(fp2, "%10.2f", distance[i - 1][j - 1]);
			fprintf(fp15, "%10.2f", 1.2 * distance[i - 1][j - 1]);
		}
		fprintf(fp2, "\n");
		fprintf(fp15, "\n");

		fprintf(fp3, "%10.2f\n", RandomRge(weight_lb, weight_ub));
		fprintf(fp4, "%10.2f\n", RandomRge(revenue_lb, revenue_ub));

		fprintf(fp6, "%10.0f\n", RandomRge(cap_lb, cap_ub));

		fprintf(fp9, "%10.2f\n", RandomRge(settime0_lb, settime0_ub));

		for (int j = 1; j <= N_Order; j++)
		{
			if (i == j)
				settime = 0.0;
			else
				settime = RandomRge(settime1_lb, settime1_ub);
			fprintf(fp10, "%10.2f", settime);
		}
		fprintf(fp10, "\n");

		fprintf(fp11, "%10.2f\n", RandomRge(proctime_lb, proctime_ub));
		fprintf(fp12, "%10.2f\n", RandomRge(setcost0_lb, setcost0_ub));

		for (int j = 1; j <= N_Order; j++)
		{
			if (i == j)
				setcost = 0.0;
			else
				setcost = RandomRge(setcost1_lb, setcost1_ub);
			fprintf(fp13, "%10.2f", setcost);
		}
		fprintf(fp13, "\n");
		tmpvv = RandomRge(duedate0_lb, duedate0_ub);
		fprintf(fp7, "%10.2f\n", tmpvv);
		if (tmpvv < duedate1_lb)
			fprintf(fp8, "%10.2f\n", RandomRge(duedate1_lb, duedate1_ub));
		else
			fprintf(fp8, "%10.2f\n", RandomRge(tmpvv, duedate1_ub));
	}

	fclose(fp1);
	fclose(fp2);
	fclose(fp3);
	fclose(fp4);

	fclose(fp6);
	fclose(fp7);
	fclose(fp8);
	fclose(fp9);
	fclose(fp10);
	fclose(fp11);
	fclose(fp12);
	fclose(fp13);
	fclose(fp14);
	fclose(fp15);

	fp3 = fopen("weight.txt", "r"); int i = 0;
	for (int j = 1; fgets(buf, 11, fp3) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			weight[i - 1] = (float)atof(buf);
		}
	}
	fclose(fp3);

	vehicle_size = size;

	fp6 = fopen("cap.txt", "r");  i = 0;
	for (int j = 1; fgets(buf, 11, fp6) != NULL; j++)
	{

		if (j % 2 != 0)
		{
			i = i + 1;
			order_size[i - 1] = (int)atoi(buf);
		}
	}
	fclose(fp6);


	fp7 = fopen("tw_0.txt", "r");  i = 0;
	for (int j = 1; fgets(buf, 11, fp7) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			duedate0[i - 1] = (float)atof(buf);
		}
	}

	fclose(fp7);
	fp4 = fopen("revenue.txt", "r"); i = 0;
	for (int j = 1; fgets(buf, 11, fp4) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			revenue[i - 1] = (float)atof(buf);
		}
	}
	fclose(fp4);

	fp8 = fopen("tw_1.txt", "r"); i = 0;
	for (int j = 1; fgets(buf, 11, fp8) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			duedate1[i - 1] = (float)atof(buf);
		}
	}

	fclose(fp8);

	fp9 = fopen("settime_0.txt", "r"); i = 0;
	for (int j = 1; fgets(buf, 11, fp9) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			settime0[i - 1] = (float)atof(buf);
		}
	}

	fclose(fp9);
	settime1 = new float* [N_Order];
	for (i = 1; i <= N_Order; i++)
		settime1[i - 1] = new float[N_Order];

	fp10 = fopen("settime_1.txt", "r");   count_dis = 0;
	while (fgets(buf, 11, fp10) != NULL)
	{
		count_dis = count_dis + 1;
		int i = (int)count_dis % (N_Order + 1);
		int j = 1 + (int)count_dis / (N_Order + 1);
		if (i != 0)
			settime1[j - 1][i - 1] = (float)atof(buf);
	}

	fclose(fp10);

	fp11 = fopen("processtime.txt", "r"); i = 0;
	for (int j = 1; fgets(buf, 11, fp11) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			proctime[i - 1] = (float)atof(buf);
		}
	}

	fclose(fp11);


	fp12 = fopen("setcost_0.txt", "r"); i = 0;
	for (int j = 1; fgets(buf, 11, fp12) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			setcost0[i - 1] = (float)atof(buf);
		}
	}

	fclose(fp12);
	setcost1 = new float* [N_Order];
	for (int i = 1; i <= N_Order; i++)
		setcost1[i - 1] = new float[N_Order];
	fp13 = fopen("setcost_1.txt", "r");   count_dis = 0;
	while (fgets(buf, 11, fp13) != NULL)
	{
		count_dis = count_dis + 1;
		int i = (int)count_dis % (N_Order + 1);
		int j = 1 + (int)count_dis / (N_Order + 1);
		if (i != 0)
			setcost1[j - 1][i - 1] = (float)atof(buf);
	}

	fclose(fp13);

	fp14 = fopen("traveltime_0.txt", "r"); i = 0;
	for (int j = 1; fgets(buf, 11, fp14) != NULL; j++)
	{
		if (j % 2 != 0)
		{
			i = i + 1;
			traveltime0[i - 1] = (float)atof(buf);
		}
	}

	fclose(fp14);

	traveltime1 = new float* [N_Order];
	for (int i = 1; i <= N_Order; i++)
		traveltime1[i - 1] = new float[N_Order];


	fp15 = fopen("traveltime_1.txt", "r");   count_dis = 0;
	while (fgets(buf, 11, fp15) != NULL)
	{
		count_dis = count_dis + 1;
		int i = (int)count_dis % (N_Order + 1);
		int j = 1 + (int)count_dis / (N_Order + 1);
		if (i != 0)
			traveltime1[j - 1][i - 1] = (float)atof(buf);
	}

	fclose(fp15);

	t2_0 = new float[N_Vehicle];
	Curr.M_select = new int[N_Order]; Curr.V_select = new int[N_Order]; Curr.Order1_seq = new int[N_Order];  Curr.Order2_seq = new int[N_Order];
	Curr1.M_select = new int[N_Order]; Curr1.V_select = new int[N_Order]; Curr1.Order1_seq = new int[N_Order]; Curr1.Order2_seq = new int[N_Order];
	Curr2.M_select = new int[N_Order]; Curr2.V_select = new int[N_Order];  Curr2.Order1_seq = new int[N_Order];   Curr2.Order2_seq = new int[N_Order];
	Best1.M_select = new int[N_Order]; Best1.V_select = new int[N_Order]; Best1.Order1_seq = new int[N_Order]; Best1.Order2_seq = new int[N_Order];
	Curr.M_selectval = new float[N_Order]; Curr.V_selectval = new float[N_Order]; Curr.Order1_seqval = new float[N_Order]; Curr.Order2_seqval = new float[N_Order];
	Curr1.M_selectval = new float[N_Order]; Curr1.V_selectval = new float[N_Order]; Curr1.Order1_seqval = new float[N_Order]; Curr1.Order2_seqval = new float[N_Order];
	Curr2.M_selectval = new float[N_Order]; Curr2.V_selectval = new float[N_Order]; Curr2.Order1_seqval = new float[N_Order]; Curr2.Order2_seqval = new float[N_Order];
	Best1.M_selectval = new float[N_Order]; Best1.V_selectval = new float[N_Order]; Best1.Order1_seqval = new float[N_Order]; Best1.Order2_seqval = new float[N_Order];
	Curr.M_velocity = new float[N_Order];  Curr.V_velocity = new float[N_Order]; Curr.Order1_velocity = new float[N_Order];  Curr.Order2_velocity = new float[N_Order];
	Curr1.M_velocity = new float[N_Order];  Curr1.V_velocity = new float[N_Order]; Curr1.Order1_velocity = new float[N_Order];  Curr1.Order2_velocity = new float[N_Order];
	Curr2.M_velocity = new float[N_Order];  Curr2.V_velocity = new float[N_Order]; Curr2.Order1_velocity = new float[N_Order];  Curr2.Order2_velocity = new float[N_Order];
	Best1.M_velocity = new float[N_Order];  Best1.V_velocity = new float[N_Order]; Best1.Order1_velocity = new float[N_Order];  Best1.Order2_velocity = new float[N_Order];
	PSO = new struct bettersolu[N_Particle]; PB = new struct bettersolu[N_Particle];
	for (int i = 1; i <= N_Particle; i++)
	{
		PSO[i - 1].M_select = new int[N_Order];  PSO[i - 1].V_select = new int[N_Order];  PSO[i - 1].Order1_seq = new int[N_Order];  PSO[i - 1].Order2_seq = new int[N_Order];
		PSO[i - 1].M_selectval = new float[N_Order]; PSO[i - 1].V_selectval = new float[N_Order];  PSO[i - 1].Order1_seqval = new float[N_Order]; PSO[i - 1].Order2_seqval = new float[N_Order];
		PSO[i - 1].M_velocity = new float[N_Order]; PSO[i - 1].V_velocity = new float[N_Order];  PSO[i - 1].Order1_velocity = new float[N_Order]; PSO[i - 1].Order2_velocity = new float[N_Order];

		PB[i - 1].M_select = new int[N_Order];  PB[i - 1].V_select = new int[N_Order];  PB[i - 1].Order1_seq = new int[N_Order];  PB[i - 1].Order2_seq = new int[N_Order];
		PB[i - 1].M_selectval = new float[N_Order]; PB[i - 1].V_selectval = new float[N_Order];  PB[i - 1].Order1_seqval = new float[N_Order]; PB[i - 1].Order2_seqval = new float[N_Order];
		PB[i - 1].M_velocity = new float[N_Order]; PB[i - 1].V_velocity = new float[N_Order]; PB[i - 1].Order1_velocity = new float[N_Order]; PB[i - 1].Order2_velocity = new float[N_Order];

	}

	GB.M_select = new int[N_Order];  GB.V_select = new int[N_Order];  GB.Order1_seq = new int[N_Order];  GB.Order2_seq = new int[N_Order];
	GB.M_selectval = new float[N_Order]; GB.V_selectval = new float[N_Order];  GB.Order1_seqval = new float[N_Order]; GB.Order2_seqval = new float[N_Order];

	Test.M_select = new int[N_Order];  Test.V_select = new int[N_Order];  Test.Order1_seq = new int[N_Order];  Test.Order2_seq = new int[N_Order];
	Test.M_selectval = new float[N_Order]; Test.V_selectval = new float[N_Order];  Test.Order1_seqval = new float[N_Order]; Test.Order2_seqval = new float[N_Order];
	Test.M_velocity = new float[N_Order]; Test.V_velocity = new float[N_Order]; Test.Order1_velocity = new float[N_Order]; Test.Order2_velocity = new float[N_Order];

	DEV.M_select = new int[N_Order];  DEV.V_select = new int[N_Order];  DEV.Order1_seq = new int[N_Order];  DEV.Order2_seq = new int[N_Order];
	DEV.M_selectval = new float[N_Order]; DEV.V_selectval = new float[N_Order];  DEV.Order1_seqval = new float[N_Order]; DEV.Order2_seqval = new float[N_Order];
	DEV.M_velocity = new float[N_Order]; DEV.V_velocity = new float[N_Order]; DEV.Order1_velocity = new float[N_Order]; DEV.Order2_velocity = new float[N_Order];

	DEV.M_select = new int[N_Order];  DEV.V_select = new int[N_Order];  DEV.Order1_seq = new int[N_Order];  DEV.Order2_seq = new int[N_Order];
	DEV.M_selectval = new float[N_Order]; DEV.V_selectval = new float[N_Order];  DEV.Order1_seqval = new float[N_Order]; DEV.Order2_seqval = new float[N_Order];
	DEV.M_velocity = new float[N_Order]; DEV.V_velocity = new float[N_Order]; DEV.Order1_velocity = new float[N_Order]; DEV.Order2_velocity = new float[N_Order];

	DEU.M_select = new int[N_Order];  DEU.V_select = new int[N_Order];  DEU.Order1_seq = new int[N_Order];  DEU.Order2_seq = new int[N_Order];
	DEU.M_selectval = new float[N_Order]; DEU.V_selectval = new float[N_Order];  DEU.Order1_seqval = new float[N_Order]; DEU.Order2_seqval = new float[N_Order];
	DEU.M_velocity = new float[N_Order]; DEU.V_velocity = new float[N_Order]; DEU.Order1_velocity = new float[N_Order]; DEU.Order2_velocity = new float[N_Order];


	M_select = new int[N_Order];    Order1_seq = new int[N_Order];     V_select = new int[N_Order];   Order2_seq = new int[N_Order];
	M_selectval = new float[N_Order]; Order1_seqval = new float[N_Order]; V_selectval = new float[N_Order]; Order2_seqval = new float[N_Order];
	M_velocity = new float[N_Order]; Order1_velocity = new float[N_Order]; V_velocity = new float[N_Order]; Order2_velocity = new float[N_Order];
	count_m = new int[N_Machine]; count_v = new int[N_Vehicle]; t1_tmp = new float[N_Order]; t2_tmp = new float[N_Order];
	mach_order_seq = new int* [N_Machine];
	mach_order_time = new float* [N_Machine];
	for (int i = 1; i <= N_Machine; i++)
	{
		mach_order_seq[i - 1] = new int[N_Order];
		mach_order_time[i - 1] = new float[N_Order];
	}

	vehicle_order_seq = new int* [N_Vehicle];
	vehicle_order_time = new float* [N_Vehicle];
	for (int i = 1; i <= N_Vehicle; i++)
	{
		vehicle_order_seq[i - 1] = new int[N_Order];
		vehicle_order_time[i - 1] = new float[N_Order];
	}
	order_finishtime = new float[N_Order];
	order_finishtime_orderno = new int[N_Order];
	mach_usage_time = new float[N_Machine];
	mach_previousjob = new int[N_Machine];
	avgproctime = new float[N_Order];
	avgtraveltime = new float[N_Order];
	avgtime = new float[N_Order];



}


void random_initsol()
{
	int tmpindex1, tmpindex2, count_cycle = 0;
	float tmpval1;

reinitialsolu:
	start1 = clock();

	for (int i = 1; i <= N_Order; i++)
	{
		Order1_seqval[i - 1] = RandomRge(0.0, (float)(N_Order + 1 - 0.00001));
		Order2_seqval[i - 1] = RandomRge(0.0, (float)(N_Order + 1 - 0.00001));
		M_selectval[i - 1] = RandomRge(1.0, (float)(N_Machine + 1 - 0.00001));
		V_selectval[i - 1] = RandomRge(1.0, (float)(N_Vehicle + 1 - 0.00001));
		/*		Order1_velocity[i-1]=RandomRge((float)(-N_Order-1+0.00001),(float)(N_Order+1-0.00001));
		Order2_velocity[i-1]=RandomRge((float)(-N_Order-1+0.00001),(float)(N_Order+1-0.00001));
		M_velocity[i-1]=RandomRge((float)(-N_Machine-1+0.00001),(float)(N_Machine+1-0.00001));
		V_velocity[i-1]=RandomRge((float)(-N_Vehicle-1+0.00001),(float)(N_Vehicle+1-0.00001));
		*/		M_select[i - 1] = (int)M_selectval[i - 1];
		V_select[i - 1] = (int)V_selectval[i - 1];
		Order1_seq[i - 1] = i;
		Order2_seq[i - 1] = i;
	}



	for (int i = 1; i <= (N_Order - 1); i++)
		for (int j = (i + 1); j <= (N_Order); j++)
		{
			if (i != j)
			{
				if (Order1_seqval[i - 1] > Order1_seqval[j - 1])
				{
					tmpval1 = Order1_seqval[i - 1];  Order1_seqval[i - 1] = Order1_seqval[j - 1];  Order1_seqval[j - 1] = tmpval1;
					tmpindex1 = Order1_seq[i - 1];   Order1_seq[i - 1] = Order1_seq[j - 1];    Order1_seq[j - 1] = tmpindex1;
					tmpval1 = M_selectval[i - 1];   M_selectval[i - 1] = M_selectval[j - 1];  M_selectval[j - 1] = tmpval1;
					tmpindex2 = M_select[i - 1];    M_select[i - 1] = M_select[j - 1];        M_select[j - 1] = tmpindex2;
				}
				if (Order2_seqval[i - 1] > Order2_seqval[j - 1])
				{
					tmpval1 = Order2_seqval[i - 1]; Order2_seqval[i - 1] = Order2_seqval[j - 1];  Order2_seqval[j - 1] = tmpval1;
					tmpindex1 = Order2_seq[i - 1];  Order2_seq[i - 1] = Order2_seq[j - 1];    Order2_seq[j - 1] = tmpindex1;
					tmpval1 = V_selectval[i - 1];   V_selectval[i - 1] = V_selectval[j - 1];  V_selectval[j - 1] = tmpval1;
					tmpindex2 = V_select[i - 1];    V_select[i - 1] = V_select[j - 1];        V_select[j - 1] = tmpindex2;
				}
			}
		}





	for (int iii = 1; iii <= N_Vehicle; iii++)
	{
		tmp_vehicle_size = 0;
		for (int jjj = 1; jjj <= N_Order; jjj++)
		{
			if (V_select[jjj - 1] == iii)
				tmp_vehicle_size += order_size[jjj - 1];
		}
		if (tmp_vehicle_size > vehicle_size)
		{
			count_cycle++;
			if (count_cycle > 1000)
			{
				printf("error");
				system("pause");
			}
			goto reinitialsolu;


		}
	}

	for (int i = 1; i <= N_Order; i++)
	{
		Curr.Order1_seq[i - 1] = Order1_seq[i - 1];	Curr.M_select[i - 1] = M_select[i - 1];
		Curr.Order2_seq[i - 1] = Order2_seq[i - 1];	Curr.V_select[i - 1] = V_select[i - 1];
		Curr.Order1_seqval[i - 1] = Order1_seqval[i - 1]; Curr.M_selectval[i - 1] = M_selectval[i - 1];
		Curr.Order2_seqval[i - 1] = Order2_seqval[i - 1]; Curr.V_selectval[i - 1] = V_selectval[i - 1];
	}

}

void CalObj()
{

	float tardiness;

	int  temp_m, temp_o, temp_v, temp_order2, temp_order1;



	for (int i = 1; i <= N_Machine; i++)
		count_m[i - 1] = 0;

	for (int i = 1; i <= N_Order; i++)
	{

		temp_m = M_select[i - 1];    //temp_m : 排程部分第i訂單機台安排
		count_m[temp_m - 1] += 1;    //第temp_m機台目前訂單數量 
		temp_o = count_m[temp_m - 1]; //第temp_m機台目前訂單數量
		mach_order_seq[temp_m - 1][temp_o - 1] = Order1_seq[i - 1];  //第temp_m機台的第temp_o個訂單的訂單編號
		temp_order2 = mach_order_seq[temp_m - 1][temp_o - 1];  //第temp_m機台的第temp_o個訂單的訂單編號

		if (temp_o == 1)
		{
			mach_order_time[temp_m - 1][temp_o - 1] = settime0[temp_order2 - 1] + proctime[temp_order2 - 1];

		}
		else
		{
			temp_order1 = mach_order_seq[temp_m - 1][temp_o - 2];
			mach_order_time[temp_m - 1][temp_o - 1] = mach_order_time[temp_m - 1][temp_o - 2] + settime1[temp_order1 - 1][temp_order2 - 1] + proctime[temp_order2 - 1];


		}
		t1_tmp[temp_order2 - 1] = mach_order_time[temp_m - 1][temp_o - 1];

	}

	for (int i = 1; i <= N_Vehicle; i++)
		count_v[i - 1] = 0;

	for (int i = 1; i <= N_Vehicle; i++)
		t2_0[i - 1] = (float)0.0;

	for (int i = 1; i <= N_Order; i++)
	{
		temp_v = V_select[i - 1];
		temp_order2 = Order2_seq[i - 1];

		if (t2_0[temp_v - 1] < t1_tmp[temp_order2 - 1])
			t2_0[temp_v - 1] = t1_tmp[temp_order2 - 1];
	}


	for (int i = 1; i <= N_Order; i++)
	{
		temp_v = V_select[i - 1];
		count_v[temp_v - 1] += 1;
		temp_o = count_v[temp_v - 1];
		vehicle_order_seq[temp_v - 1][temp_o - 1] = Order2_seq[i - 1];
		temp_order2 = vehicle_order_seq[temp_v - 1][temp_o - 1];

		if (temp_o == 1)
		{
			vehicle_order_time[temp_v - 1][temp_o - 1] = t2_0[temp_v - 1] + traveltime0[temp_order2 - 1];


		}
		else
		{
			temp_order1 = vehicle_order_seq[temp_v - 1][temp_o - 2];
			vehicle_order_time[temp_v - 1][temp_o - 1] = vehicle_order_time[temp_v - 1][temp_o - 2] + traveltime1[temp_order1 - 1][temp_order2 - 1];

		}
		if (vehicle_order_time[temp_v - 1][temp_o - 1] < duedate0[temp_order2 - 1])
			vehicle_order_time[temp_v - 1][temp_o - 1] = duedate0[temp_order2 - 1];
		t2_tmp[temp_order2 - 1] = vehicle_order_time[temp_v - 1][temp_o - 1];


	}

	curr_obj = (float)0.0;
	for (int i = 1; i <= N_Order; i++)
	{
		tardiness = t2_tmp[i - 1] - duedate1[i - 1];
		if (tardiness > 0)
			curr_obj += weight[i - 1] * tardiness;
	}

}

void VNS1()
{
	int improve1_count, improve11 = 0, improve12 = 0, improve13 = 0, improve14 = 0, count144;
	int  count44, improve24 = 0, improve2_count, improve21 = 0, improve22 = 0, improve23 = 0;
	improve1_count = 0;


rerun1:
	for (int a = 1; a <= (N_Order + 1) * N_Order; a++)
	{
		int k11 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reselect11:
		int k12 = (int)RandomRge((float)1.0, (float)(N_Order + 2 - 0.00001));
		if (k11 == k12 || k11 == (k12 - 1))
			goto reselect11;

		if (k11 > k12)
		{
			for (int q11 = k11; q11 >= (k12 + 1); q11--)
			{
				Order1_seq[q11 - 1] = Curr1.Order1_seq[q11 - 2];
				Order1_seqval[q11 - 1] = Curr1.Order1_seqval[q11 - 2];
				Order1_velocity[q11 - 1] = Curr1.Order1_velocity[q11 - 2];
				M_select[q11 - 1] = Curr1.M_select[q11 - 2];
				M_selectval[q11 - 1] = Curr1.M_selectval[q11 - 2];
				M_velocity[q11 - 1] = Curr1.M_velocity[q11 - 2];
			}
			Order1_seq[k12 - 1] = Curr1.Order1_seq[k11 - 1];
			if (k12 == 1)
				Order1_seqval[k12 - 1] = RandomRge((float)0.0, Curr1.Order1_seqval[k12 - 1]);
			else
				Order1_seqval[k12 - 1] = RandomRge(Curr1.Order1_seqval[k12 - 2], Curr1.Order1_seqval[k12 - 1]);
			Order1_velocity[k12 - 1] = Curr1.Order1_velocity[k11 - 1];

			M_select[k12 - 1] = Curr1.M_select[k11 - 1];
			M_selectval[k12 - 1] = Curr1.M_selectval[k11 - 1];
			M_velocity[k12 - 1] = Curr1.M_velocity[k11 - 1];

		}
		else
		{
			for (int q11 = k11; q11 <= (k12 - 2); q11++)
			{
				Order1_seq[q11 - 1] = Curr1.Order1_seq[q11];
				Order1_seqval[q11 - 1] = Curr1.Order1_seqval[q11];
				Order1_velocity[q11 - 1] = Curr1.Order1_velocity[q11];
				M_select[q11 - 1] = Curr1.M_select[q11];
				M_selectval[q11 - 1] = Curr1.M_selectval[q11];
				M_velocity[q11 - 1] = Curr1.M_velocity[q11];
			}
			Order1_seq[k12 - 2] = Curr1.Order1_seq[k11 - 1];
			if (k12 == (N_Order + 1))
				Order1_seqval[k12 - 2] = RandomRge(Curr1.Order1_seqval[k12 - 2], (float)(N_Order + 1 - 0.00001));
			else
				Order1_seqval[k12 - 2] = RandomRge(Curr1.Order1_seqval[k12 - 2], Curr1.Order1_seqval[k12 - 1]);
			Order1_velocity[k12 - 2] = Curr1.Order1_velocity[k11 - 1];
			M_select[k12 - 2] = Curr1.M_select[k11 - 1];
			M_selectval[k12 - 2] = Curr1.M_selectval[k11 - 1];
			M_velocity[k12 - 2] = Curr1.M_velocity[k11 - 1];
		}

		CalObj();
		if (curr_obj < best_obj)
		{
			Curr1.obj = curr_obj;
			if (curr_obj < Best1.obj)
			{
				check++;
				Best1.obj = curr_obj;
				for (int q2 = 1; q2 <= N_Order; q2++)
				{
					Best1.M_select[q2 - 1] = M_select[q2 - 1];
					Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
					Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
					Best1.V_select[q2 - 1] = V_select[q2 - 1];
					Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
					Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
					Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
					Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
					Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];
					Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
					Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
					Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
				}
				now_time = timing(start1);
				if (Curr1.obj < PB[samea - 1].obj)
				{
					if (Curr1.obj < GB.obj)
					{
						printf("check=%d  VNS1-1 iter=%d particle=%d", check, iteration, samea);
						printf("     obj=%f", Curr1.obj);
						printf("     time=%.4f\n", now_time);
					}
				}
				fprintf(fp16, "check=%d  VNS1-1  iter=%d", check, iteration);
				fprintf(fp16, "     obj=%f", Curr1.obj);
				fprintf(fp16, "     time=%.4f", now_time);
			}
			best_obj = curr_obj;
			for (int q2 = 1; q2 <= N_Order; q2++)
			{
				Curr1.M_select[q2 - 1] = M_select[q2 - 1];
				Curr1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
				Curr1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
				Curr1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
				Curr1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
				Curr1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];
			}
			improve11++;
			improve1_count++;
		}
		else
		{
			curr_obj = best_obj;
			for (int q2 = 1; q2 <= N_Order; q2++)
			{
				M_select[q2 - 1] = Curr1.M_select[q2 - 1];
				M_selectval[q2 - 1] = Curr1.M_selectval[q2 - 1];
				M_velocity[q2 - 1] = Curr1.M_velocity[q2 - 1];
				Order1_seq[q2 - 1] = Curr1.Order1_seq[q2 - 1];
				Order1_seqval[q2 - 1] = Curr1.Order1_seqval[q2 - 1];
				Order1_velocity[q2 - 1] = Curr1.Order1_velocity[q2 - 1];
			}
		}
	}
	for (int aa = 1; aa <= N_Order && N_Machine != 1; aa++)
	{
		int a = (int)RandomRge(1.0, (float)(N_Order + 1 - 0.00001));

		for (int kk = 1; kk <= N_Machine; kk++)
		{
			M_selectval[a - 1] = (Curr1.M_selectval[a - 1] - (float)Curr1.M_select[a - 1]) + (float)kk;
			M_select[a - 1] = kk;
			M_velocity[a - 1] = Curr1.M_velocity[a - 1];

			CalObj();
			if (curr_obj < best_obj)
			{
				Curr1.obj = curr_obj;
				if (curr_obj < Best1.obj)
				{
					Best1.obj = curr_obj;
					check++;
					for (int q2 = 1; q2 <= N_Order; q2++)
					{
						Best1.M_select[q2 - 1] = M_select[q2 - 1];
						Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
						Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
						Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
						Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
						Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];
						Best1.V_select[q2 - 1] = V_select[q2 - 1];
						Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
						Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
						Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
						Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
						Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
					}
					now_time = timing(start1);
					if (Curr1.obj < PB[samea - 1].obj)
					{
						if (Curr1.obj < GB.obj)
						{
							printf("check=%d  VNS1-2 iter=%d particle=%d", check, iteration, samea);
							printf("     obj=%f", Curr1.obj);
							printf("     time=%.4f\n", now_time);
						}
					}
					fprintf(fp16, "check=%d  VNS1-2  iter=%d", check, iteration);
					fprintf(fp16, "    obj=%f", Curr1.obj);
					fprintf(fp16, "    time=%.4f", now_time);
				}
				best_obj = curr_obj;
				Curr1.M_select[a - 1] = M_select[a - 1];
				Curr1.M_selectval[a - 1] = M_selectval[a - 1];
				Curr1.M_velocity[a - 1] = M_velocity[a - 1];
				improve12++;
				improve1_count++;
			}
			else
			{
				curr_obj = best_obj;
				M_select[a - 1] = Curr1.M_select[a - 1];
				M_selectval[a - 1] = Curr1.M_selectval[a - 1];
				M_velocity[a - 1] = Curr1.M_velocity[a - 1];
			}
		}
	}
	for (int a = 1; a <= N_Order * N_Order; a++)
	{

		int k11 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reselect12:
		int k12 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));

		if (k11 == k12)
			goto reselect12;
		Order1_seq[k11 - 1] = Curr1.Order1_seq[k12 - 1];

		Order1_seq[k12 - 1] = Curr1.Order1_seq[k11 - 1];

		CalObj();
		if (curr_obj < best_obj)
		{
			Curr1.obj = curr_obj;
			if (curr_obj < Best1.obj)
			{
				Best1.obj = curr_obj;
				check++;
				for (int q2 = 1; q2 <= N_Order; q2++)
				{
					Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
					Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
					Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];
					Best1.M_select[q2 - 1] = M_select[q2 - 1];
					Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
					Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
					Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
					Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
					Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
					Best1.V_select[q2 - 1] = V_select[q2 - 1];
					Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
					Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
				}
				now_time = timing(start1);
				if (Curr1.obj < PB[samea - 1].obj)
				{
					if (Curr1.obj < GB.obj)
					{
						printf("check=%d  VNS1-2 iter=%d particle=%d", check, iteration, samea);
						printf("     obj=%f", Curr1.obj);
						printf("     time=%.4f\n", now_time);
					}
				}
				fprintf(fp16, "check=%d  VNS1-2  iter=%d", check, iteration);
				fprintf(fp16, "    obj=%f", Curr1.obj);
				fprintf(fp16, "    time=%.4f", now_time);
			}
			best_obj = curr_obj;
			Curr1.Order1_seq[k11 - 1] = Order1_seq[k11 - 1];
			Curr1.Order1_seqval[k11 - 1] = Order1_seqval[k11 - 1];
			Curr1.Order1_velocity[k11 - 1] = Order1_velocity[k11 - 1];
			Curr1.Order1_seq[k12 - 1] = Order1_seq[k12 - 1];
			Curr1.Order1_seqval[k12 - 1] = Order1_seqval[k12 - 1];
			Curr1.Order1_velocity[k12 - 1] = Order1_velocity[k12 - 1];
			improve13++;
			improve1_count++;
		}
		else
		{
			curr_obj = best_obj;
			Order1_seq[k11 - 1] = Curr1.Order1_seq[k11 - 1];
			Order1_seqval[k11 - 1] = Curr1.Order1_seqval[k11 - 1];
			Order1_velocity[k11 - 1] = Curr1.Order1_velocity[k11 - 1];
			Order1_seq[k12 - 1] = Curr1.Order1_seq[k12 - 1];
			Order1_seqval[k12 - 1] = Curr1.Order1_seqval[k12 - 1];
			Order1_velocity[k12 - 1] = Curr1.Order1_velocity[k12 - 1];
		}
	}
	for (int a = 1; a <= (N_Machine + 1) * N_Machine && N_Machine != 1; a++)
	{
		count144 = 0;

		int k11 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reselect13:
		int k12 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));

		if ((k11 == k12) || (Curr1.M_select[k11 - 1] == Curr1.M_select[k12 - 1]))
		{
			count144++;
			if (count144 == ((N_Machine + 1) * N_Machine))
				break;
			goto reselect13;
		}
		M_select[k11 - 1] = Curr1.M_select[k12 - 1];
		M_selectval[k11 - 1] = Curr1.M_selectval[k12 - 1];
		M_velocity[k11 - 1] = Curr1.M_velocity[k12 - 1];
		M_select[k12 - 1] = Curr1.M_select[k11 - 1];
		M_selectval[k12 - 1] = Curr1.M_selectval[k11 - 1];
		M_velocity[k12 - 1] = Curr1.M_velocity[k11 - 1];
		CalObj();
		if (curr_obj < best_obj)
		{
			Curr1.obj = curr_obj;
			if (curr_obj < Best1.obj)
			{
				Best1.obj = curr_obj;
				check++;
				for (int q2 = 1; q2 <= N_Order; q2++)
				{
					Best1.M_select[q2 - 1] = M_select[q2 - 1];
					Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
					Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
					Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
					Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
					Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];
					Best1.V_select[q2 - 1] = V_select[q2 - 1];
					Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
					Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
					Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
					Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
					Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
				}

				now_time = timing(start1);
				if (Curr1.obj < PB[samea - 1].obj)
				{
					if (Curr1.obj < GB.obj)
					{
						printf("check=%d  VNS1-2 iter=%d particle=%d", check, iteration, samea);
						printf("     obj=%f", Curr1.obj);
						printf("     time=%.4f\n", now_time);
					}
				}
				fprintf(fp16, "check=%d  VNS1-2  iter=%d", check, iteration);
				fprintf(fp16, "    obj=%f", Curr1.obj);
				fprintf(fp16, "    time=%.4f", now_time);
			}

			best_obj = curr_obj;
			Curr1.M_select[k11 - 1] = M_select[k11 - 1];
			Curr1.M_selectval[k11 - 1] = M_selectval[k11 - 1];
			Curr1.M_velocity[k11 - 1] = M_velocity[k11 - 1];
			Curr1, M_select[k12 - 1] = M_select[k12 - 1];
			Curr1.M_selectval[k12 - 1] = M_selectval[k12 - 1];
			Curr1.M_velocity[k12 - 1] = M_velocity[k12 - 1];
			improve14++;
			improve1_count++;
		}
		else
		{
			curr_obj = best_obj;
			M_select[k11 - 1] = Curr1.M_select[k11 - 1];
			M_selectval[k11 - 1] = Curr1.M_selectval[k11 - 1];
			M_velocity[k11 - 1] = Curr1.M_velocity[k11 - 1];
			M_select[k12 - 1] = Curr1.M_select[k12 - 1];
			M_selectval[k12 - 1] = Curr1.M_selectval[k12 - 1];
			M_velocity[k12 - 1] = Curr1.M_velocity[k12 - 1];
		}

	}
	improve2_count = 0;

	for (int j = 1; j <= (N_Order + 1) * N_Order; j++)
	{

		int k1 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reselect21:
		int k2 = (int)RandomRge((float)1.0, (float)(N_Order + 2 - 0.00001));

		if (k1 == k2 || k1 == (k2 - 1))
			goto reselect21;

		if (k1 > k2)
		{
			for (int q1 = k1; q1 >= (k2 + 1); q1--)
			{
				Order2_seq[q1 - 1] = Curr1.Order2_seq[q1 - 2];
				Order2_seqval[q1 - 1] = Curr1.Order2_seqval[q1 - 2];
				Order2_velocity[q1 - 1] = Curr1.Order2_velocity[q1 - 2];
				V_select[q1 - 1] = Curr1.V_select[q1 - 2];
				V_selectval[q1 - 1] = Curr1.V_selectval[q1 - 2];
				V_velocity[q1 - 1] = Curr1.V_velocity[q1 - 2];
			}
			Order2_seq[k2 - 1] = Curr1.Order2_seq[k1 - 1];
			if (k2 == 1)
				Order2_seqval[k2 - 1] = RandomRge((float)0.0, Curr1.Order2_seqval[k2 - 1]);
			else
				Order2_seqval[k2 - 1] = RandomRge(Curr1.Order2_seqval[k2 - 2], Curr1.Order2_seqval[k2 - 1]);
			Order2_velocity[k2 - 1] = Curr1.Order2_velocity[k1 - 1];
			V_select[k2 - 1] = Curr1.V_select[k1 - 1];
			V_selectval[k2 - 1] = Curr1.V_selectval[k1 - 1];
			V_velocity[k2 - 1] = Curr1.V_velocity[k1 - 1];
		}
		else
		{
			for (int q1 = k1; q1 <= (k2 - 2); q1++)
			{
				Order2_seq[q1 - 1] = Curr1.Order2_seq[q1];
				Order2_seqval[q1 - 1] = Curr1.Order2_seqval[q1];
				Order2_velocity[q1 - 1] = Curr1.Order2_velocity[q1];
				V_select[q1 - 1] = Curr1.V_select[q1];
				V_selectval[q1 - 1] = Curr1.V_selectval[q1];
				V_velocity[q1 - 1] = Curr1.V_velocity[q1];
			}
			Order2_seq[k2 - 2] = Curr1.Order2_seq[k1 - 1];
			if (k2 == (N_Order + 1))
				Order2_seqval[k2 - 2] = RandomRge(Curr1.Order2_seqval[k2 - 2], (float)(N_Order + 1 - 0.00001));
			else
				Order2_seqval[k2 - 2] = RandomRge(Curr1.Order2_seqval[k2 - 2], Curr1.Order2_seqval[k2 - 1]);
			Order2_velocity[k2 - 2] = Curr1.Order2_velocity[k1 - 1];
			V_select[k2 - 2] = Curr1.V_select[k1 - 1];
			V_selectval[k2 - 2] = Curr1.V_selectval[k1 - 1];
			V_velocity[k2 - 2] = Curr1.V_velocity[k1 - 1];
		}

		for (int iii = 1; iii <= N_Vehicle; iii++)
		{
			tmp_vehicle_size = 0;
			for (int jjj = 1; jjj <= N_Order; jjj++)
			{
				if (V_select[jjj - 1] == iii)
					tmp_vehicle_size += order_size[jjj - 1];
			}
			if (tmp_vehicle_size > vehicle_size)
				goto revns21;
		}

		CalObj();

		if (curr_obj < best_obj)
		{
			Curr1.obj = curr_obj;
			if (curr_obj < Best1.obj)
			{
				check++;

				Best1.obj = curr_obj;
				for (int q2 = 1; q2 <= N_Order; q2++)
				{
					Best1.V_select[q2 - 1] = V_select[q2 - 1];
					Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
					Best1.M_select[q2 - 1] = M_select[q2 - 1];
					Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
					Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
					Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
					Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
					Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
					Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
					Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
					Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
					Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];

				}
				now_time = timing(start1);
				if (Curr1.obj < PB[samea - 1].obj)
				{
					if (Curr1.obj < GB.obj)
					{
						printf("check=%d  VNS2-1 iter=%d particle=%d", check, iteration, samea);
						printf("     obj=%f", Curr1.obj);
						printf("     time=%.4f\n", now_time);
					}
				}
				fprintf(fp16, "check=%d  VNS2-1 iter=%d", check, iteration);
				fprintf(fp16, "     obj=%f", Curr1.obj);
				fprintf(fp16, "     time=%.4f\n", now_time);


			}

			best_obj = curr_obj;
			for (int q2 = 1; q2 <= N_Order; q2++)
			{
				Curr1.V_select[q2 - 1] = V_select[q2 - 1];
				Curr1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
				Curr1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
				Curr1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
				Curr1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
				Curr1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
			}

			improve21++;
			improve2_count++;


		}
		else
		{
		revns21:				curr_obj = best_obj;
			for (int q2 = 1; q2 <= N_Order; q2++)
			{
				V_select[q2 - 1] = Curr1.V_select[q2 - 1];
				V_selectval[q2 - 1] = Curr1.V_selectval[q2 - 1];
				V_velocity[q2 - 1] = Curr1.V_velocity[q2 - 1];
				Order2_seq[q2 - 1] = Curr1.Order2_seq[q2 - 1];
				Order2_seqval[q2 - 1] = Curr1.Order2_seqval[q2 - 1];
				Order2_velocity[q2 - 1] = Curr1.Order2_velocity[q2 - 1];
			}
		}
	}
	for (int jj = 1; jj <= N_Order && N_Vehicle != 1; jj++)
	{
		int j = (int)RandomRge(1.0, (float)(N_Order + 1 - 0.00001));

		for (int k = 1; k <= N_Vehicle; k++)
		{
			V_selectval[j - 1] = (Curr1.V_selectval[j - 1] - (float)Curr1.V_select[j - 1]) + (float)k;
			V_select[j - 1] = k;
			V_velocity[j - 1] = Curr1.V_velocity[j - 1];

			for (int iii = 1; iii <= N_Vehicle; iii++)
			{
				tmp_vehicle_size = 0;
				for (int jjj = 1; jjj <= N_Order; jjj++)
				{
					if (V_select[jjj - 1] == iii)
						tmp_vehicle_size += order_size[jjj - 1];
				}
				if (tmp_vehicle_size > vehicle_size)
					goto revns22;
			}

			CalObj();

			if (curr_obj < best_obj)
			{
				Curr1.obj = curr_obj;
				if (curr_obj < Best1.obj)
				{
					Best1.obj = curr_obj;
					check++;
					for (int q2 = 1; q2 <= N_Order; q2++)
					{
						Best1.V_select[q2 - 1] = V_select[q2 - 1];
						Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
						Best1.M_select[q2 - 1] = M_select[q2 - 1];
						Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
						Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
						Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
						Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
						Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
						Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
						Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
						Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
						Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];

					}
					now_time = timing(start1);
					if (Curr1.obj < PB[samea - 1].obj)
					{
						if (Curr1.obj < GB.obj)
						{
							printf("check=%d  VNS2-2 iter=%d particle=%d", check, iteration, samea);
							printf("     obj=%f", Curr1.obj);
							printf("     time=%.4f\n", now_time);
						}
					}
					fprintf(fp16, "check=%d  VNS2-2 iter=%d", check, iteration);
					fprintf(fp16, "     obj=%f", Curr1.obj);
					fprintf(fp16, "     time=%.4f\n", now_time);
				}

				best_obj = curr_obj;

				Curr1.V_select[j - 1] = V_select[j - 1];
				Curr1.V_selectval[j - 1] = V_selectval[j - 1];
				Curr1.V_velocity[j - 1] = V_velocity[j - 1];

				improve22++;
				improve2_count++;
			}
			else
			{
			revns22:					curr_obj = best_obj;
				V_select[j - 1] = Curr1.V_select[j - 1];
				V_selectval[j - 1] = Curr1.V_selectval[j - 1];
				V_velocity[j - 1] = Curr1.V_velocity[j - 1];
			}
		}
	}
	for (int j = 1; j <= N_Order * N_Order; j++)
	{
		int k1 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reselect22:
		int k2 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));

		if (k1 == k2)
			goto reselect22;

		Order2_seq[k1 - 1] = Curr1.Order2_seq[k2 - 1];
		Order2_seq[k2 - 1] = Curr1.Order2_seq[k1 - 1];
		Order2_seqval[k1 - 1] = Curr2.Order2_seqval[k2 - 1];//??
		Order2_seqval[k2 - 1] = Curr2.Order2_seqval[k1 - 1];//??
		Order2_velocity[k1 - 1] = Curr2.Order2_velocity[k2 - 1];//??
		Order2_velocity[k2 - 1] = Curr2.Order2_velocity[k1 - 1];//??

		for (int iii = 1; iii <= N_Vehicle; iii++)
		{
			tmp_vehicle_size = 0;
			for (int jjj = 1; jjj <= N_Order; jjj++)
			{
				if (V_select[jjj - 1] == iii)
					tmp_vehicle_size += order_size[jjj - 1];
			}
			if (tmp_vehicle_size > vehicle_size)
				goto revns23;
		}

		CalObj();

		if (curr_obj < best_obj)
		{
			Curr1.obj = curr_obj;
			if (curr_obj < Best1.obj)
			{
				Best1.obj = curr_obj;
				check++;
				for (int q2 = 1; q2 <= N_Order; q2++)
				{
					Best1.V_select[q2 - 1] = V_select[q2 - 1];
					Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
					Best1.M_select[q2 - 1] = M_select[q2 - 1];
					Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
					Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
					Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
					Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
					Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
					Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
					Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
					Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
					Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];

				}
				now_time = timing(start1);
				if (Curr1.obj < PB[samea - 1].obj)
				{
					if (Curr1.obj < GB.obj)
					{
						printf("check=%d  VNS2-3 iter=%d particle=%d", check, iteration, samea);
						printf("     obj=%f", Curr1.obj);
						printf("     time=%.4f\n", now_time);
					}
				}
				fprintf(fp16, "check=%d  VNS2-3 iter=%d", check, iteration);
				fprintf(fp16, "     obj=%f", Curr1.obj);
				fprintf(fp16, "     time=%.4f\n", now_time);
			}

			best_obj = curr_obj;
			Curr1.Order2_seq[k1 - 1] = Order2_seq[k1 - 1];
			Curr1.Order2_seqval[k1 - 1] = Order2_seqval[k1 - 1];
			Curr1.Order2_velocity[k1 - 1] = Order2_velocity[k1 - 1];
			Curr1.Order2_seq[k2 - 1] = Order2_seq[k2 - 1];
			Curr1.Order2_seqval[k2 - 1] = Order2_seqval[k2 - 1];
			Curr1.Order2_velocity[k2 - 1] = Order2_velocity[k2 - 1];

			improve23++;
			improve2_count++;

		}
		else
		{
		revns23:
			curr_obj = best_obj;
			Order2_seq[k1 - 1] = Curr1.Order2_seq[k1 - 1];
			Order2_seqval[k1 - 1] = Curr1.Order2_seqval[k1 - 1];
			Order2_velocity[k1 - 1] = Curr1.Order2_velocity[k1 - 1];
			Order2_seq[k2 - 1] = Curr1.Order2_seq[k2 - 1];
			Order2_seqval[k2 - 1] = Curr1.Order2_seqval[k2 - 1];
			Order2_velocity[k2 - 1] = Curr1.Order2_velocity[k2 - 1];
		}
	}

	for (int j = 1; j <= (N_Vehicle + 1) * N_Vehicle && N_Vehicle != 1; j++)
	{
		count44 = 0;

		int k1 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reselect23:
		int k2 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));

		if ((k1 == k2) || (Curr1.V_select[k1 - 1] == Curr1.V_select[k2 - 1]))
		{
			count44++;
			if (count44 == ((N_Vehicle + 1) * N_Vehicle))
				break;
			goto reselect23;
		}
		V_select[k1 - 1] = Curr1.V_select[k2 - 1];
		V_selectval[k1 - 1] = Curr1.V_selectval[k2 - 1];
		V_velocity[k1 - 1] = Curr1.V_velocity[k2 - 1];
		V_select[k2 - 1] = Curr1.V_select[k1 - 1];
		V_selectval[k2 - 1] = Curr1.V_selectval[k1 - 1];
		V_velocity[k2 - 1] = Curr1.V_velocity[k1 - 1];
		for (int iii = 1; iii <= N_Vehicle; iii++)
		{
			tmp_vehicle_size = 0;
			for (int jjj = 1; jjj <= N_Order; jjj++)
			{
				if (V_select[jjj - 1] == iii)
					tmp_vehicle_size += order_size[jjj - 1];
			}
			if (tmp_vehicle_size > vehicle_size)
				goto revns24;
		}
		CalObj();

		if (curr_obj < best_obj)
		{
			Curr1.obj = curr_obj;
			if (curr_obj < Best1.obj)
			{
				Best1.obj = curr_obj;
				check++;
				for (int q2 = 1; q2 <= N_Order; q2++)
				{
					Best1.V_select[q2 - 1] = V_select[q2 - 1];
					Best1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
					Best1.M_select[q2 - 1] = M_select[q2 - 1];
					Best1.Order1_seq[q2 - 1] = Order1_seq[q2 - 1];
					Best1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
					Best1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
					Best1.M_selectval[q2 - 1] = M_selectval[q2 - 1];
					Best1.Order1_seqval[q2 - 1] = Order1_seqval[q2 - 1];
					Best1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
					Best1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
					Best1.M_velocity[q2 - 1] = M_velocity[q2 - 1];
					Best1.Order1_velocity[q2 - 1] = Order1_velocity[q2 - 1];


				}
				now_time = timing(start1);
				if (Curr1.obj < PB[samea - 1].obj)
				{
					if (Curr1.obj < GB.obj)
					{
						printf("check=%d  VNS2-4 iter=%d particle=%d", check, iteration, samea);
						printf("     obj=%f", Curr1.obj);
						printf("     time=%.4f\n", now_time);
					}
				}
				fprintf(fp16, "check=%d  VNS2-4 iter=%d", check, iteration);
				fprintf(fp16, "     obj=%f", Curr1.obj);
				fprintf(fp16, "     time=%.4f\n", now_time);
			}

			best_obj = curr_obj;
			Curr1.V_select[k1 - 1] = V_select[k1 - 1];
			Curr1.V_selectval[k1 - 1] = V_selectval[k1 - 1];
			Curr1.V_velocity[k1 - 1] = V_velocity[k1 - 1];
			Curr1.V_select[k2 - 1] = V_select[k2 - 1];
			Curr1.V_selectval[k2 - 1] = V_selectval[k2 - 1];
			Curr1.V_velocity[k2 - 1] = V_velocity[k2 - 1];

			improve24++;
			improve2_count++;

		}
		else
		{
		revns24:				curr_obj = Curr.obj;
			V_select[k1 - 1] = Curr1.V_select[k1 - 1];
			V_selectval[k1 - 1] = Curr1.V_selectval[k1 - 1];
			V_velocity[k1 - 1] = Curr1.V_velocity[k1 - 1];
			V_select[k2 - 1] = Curr1.V_select[k2 - 1];
			V_selectval[k2 - 1] = Curr1.V_selectval[k2 - 1];
			V_velocity[k2 - 1] = Curr1.V_velocity[k2 - 1];

		}

	}
	if (improve2_count != 0)
	{
		improve2_count = 0;
		improve1_count = 0;
		goto rerun1;
	}



	if (improve1_count != 0)
	{
		improve1_count = 0;
		improve2_count = 0;
		goto rerun1;
	}

}

void shaking()
{
	int  shaking_index;

	shaking_index = (int)RandomRge((float)1.0, (float)(shaking_ratio * N_Order));

	for (int i = 1; i <= shaking_index; i++)
	{
		int k11 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));

	reselect_shaking:

		int k12 = (int)RandomRge((float)1.0, (float)(N_Order + 2 - 0.00001));
		if (k11 == k12 || k11 == (k12 - 1))
			goto reselect_shaking;

		if (k11 > k12)
		{
			for (int q11 = k11; q11 >= (k12 + 1); q11--)
			{
				Order1_seq[q11 - 1] = Curr1.Order1_seq[q11 - 2];
				Order1_seqval[q11 - 1] = Curr1.Order1_seqval[q11 - 2];
				Order1_velocity[q11 - 1] = Curr1.Order1_velocity[q11 - 2];
				M_select[q11 - 1] = Curr1.M_select[q11 - 2];
				M_selectval[q11 - 1] = Curr1.M_selectval[q11 - 2];
				M_velocity[q11 - 1] = Curr1.M_velocity[q11 - 2];
			}
			Order1_seq[k12 - 1] = Curr1.Order1_seq[k11 - 1];
			if (k12 == 1)
				Order1_seqval[k12 - 1] = RandomRge((float)0.0, Curr1.Order1_seqval[k12 - 1]);
			else
				Order1_seqval[k12 - 1] = RandomRge((float)Curr1.Order1_seqval[k12 - 2], Curr1.Order1_seqval[k12 - 1]);
			Order1_velocity[k12 - 1] = Curr1.Order1_velocity[k11 - 1];

			M_select[k12 - 1] = Curr1.M_select[k11 - 1];
			M_selectval[k12 - 1] = Curr1.M_selectval[k11 - 1];
			M_velocity[k12 - 1] = Curr1.M_velocity[k11 - 1];
		}
		else
		{
			for (int q11 = k11; q11 <= (k12 - 2); q11++)
			{
				Order1_seq[q11 - 1] = Curr1.Order1_seq[q11];
				Order1_seqval[q11 - 1] = Curr1.Order1_seqval[q11];
				Order1_velocity[q11 - 1] = Curr1.Order1_velocity[q11];
				M_select[q11 - 1] = Curr1.M_select[q11];
				M_selectval[q11 - 1] = Curr1.M_selectval[q11];
				M_velocity[q11 - 1] = Curr1.M_velocity[q11];
			}
			Order1_seq[k12 - 2] = Curr1.Order1_seq[k11 - 1];
			if (k12 == (N_Order + 1))
				Order1_seqval[k12 - 2] = RandomRge(Curr1.Order1_seqval[k12 - 2], (float)(N_Order + 1 - 0.00001));
			else
				Order1_seqval[k12 - 2] = RandomRge(Curr1.Order1_seqval[k12 - 2], Curr1.Order1_seqval[k12 - 1]);
			Order1_velocity[k12 - 2] = Curr1.Order1_velocity[k11 - 1];
			M_select[k12 - 2] = Curr1.M_select[k11 - 1];
			M_selectval[k12 - 2] = Curr1.M_selectval[k11 - 1];
			M_velocity[k12 - 2] = Curr1.M_velocity[k11 - 1];
		}

		for (int aa = 1; aa <= N_Order; aa++)
		{
			Curr1.Order1_seq[aa - 1] = Order1_seq[aa - 1];
			Curr1.Order1_seqval[aa - 1] = Order1_seqval[aa - 1];
			Curr1.Order1_velocity[aa - 1] = Order1_velocity[aa - 1];
			Curr1.M_select[aa - 1] = M_select[aa - 1];
			Curr1.M_selectval[aa - 1] = M_selectval[aa - 1];
			Curr1.M_velocity[aa - 1] = M_velocity[aa - 1];
		}
	}

	shaking_index = (int)RandomRge((float)1.0, (float)(shaking_ratio * N_Order));

	for (int j = 1; j <= int(shaking_ratio * N_Order); j++)
	{

		int k1 = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));

	reselect_shaking2:

		int k2 = (int)RandomRge((float)1.0, (float)(N_Order + 2 - 0.00001));
		if (k1 == k2 || k1 == (k2 - 1))
			goto reselect_shaking2;

		if (k1 > k2)
		{
			for (int q1 = k1; q1 >= (k2 + 1); q1--)
			{
				Order2_seq[q1 - 1] = Curr1.Order2_seq[q1 - 2];
				Order2_seqval[q1 - 1] = Curr1.Order2_seqval[q1 - 2];
				Order2_velocity[q1 - 1] = Curr1.Order2_velocity[q1 - 2];
				V_select[q1 - 1] = Curr1.V_select[q1 - 2];
				V_selectval[q1 - 1] = Curr1.V_selectval[q1 - 2];
				V_velocity[q1 - 1] = Curr1.V_velocity[q1 - 2];
			}
			Order2_seq[k2 - 1] = Curr1.Order2_seq[k1 - 1];
			if (k2 == 1)
				Order2_seqval[k2 - 1] = RandomRge((float)0.0, Curr1.Order2_seqval[k2 - 1]);
			else
				Order2_seqval[k2 - 1] = RandomRge(Curr1.Order2_seqval[k2 - 2], Curr1.Order2_seqval[k2 - 1]);
			Order2_velocity[k2 - 1] = Curr1.Order2_velocity[k1 - 1];
			V_select[k2 - 1] = Curr1.V_select[k1 - 1];
			V_selectval[k2 - 1] = Curr1.V_selectval[k1 - 1];
			V_velocity[k2 - 1] = Curr1.V_velocity[k1 - 1];
		}
		else
		{
			for (int q1 = k1; q1 <= (k2 - 2); q1++)
			{
				Order2_seq[q1 - 1] = Curr1.Order2_seq[q1];
				Order2_seqval[q1 - 1] = Curr1.Order2_seqval[q1];
				Order2_velocity[q1 - 1] = Curr1.Order2_velocity[q1];
				V_select[q1 - 1] = Curr1.V_select[q1];
				V_selectval[q1 - 1] = Curr1.V_selectval[q1];
				V_velocity[q1 - 1] = Curr1.V_velocity[q1];
			}
			Order2_seq[k2 - 2] = Curr1.Order2_seq[k1 - 1];
			if (k2 == (N_Order + 1))
				Order2_seqval[k2 - 2] = RandomRge(Curr1.Order2_seqval[k2 - 2], (float)(N_Order + 1 - 0.00001));
			else
				Order2_seqval[k2 - 2] = RandomRge(Curr1.Order2_seqval[k2 - 2], Curr1.Order2_seqval[k2 - 1]);
			Order2_velocity[k2 - 2] = Curr1.Order2_velocity[k1 - 1];
			V_select[k2 - 2] = Curr1.V_select[k1 - 1];
			V_selectval[k2 - 2] = Curr1.V_selectval[k1 - 1];
			V_velocity[k2 - 2] = Curr1.V_velocity[k1 - 1];
		}

		for (int q2 = 1; q2 <= N_Order; q2++)
		{
			Curr1.V_select[q2 - 1] = V_select[q2 - 1];
			Curr1.V_selectval[q2 - 1] = V_selectval[q2 - 1];
			Curr1.V_velocity[q2 - 1] = V_velocity[q2 - 1];
			Curr1.Order2_seq[q2 - 1] = Order2_seq[q2 - 1];
			Curr1.Order2_seqval[q2 - 1] = Order2_seqval[q2 - 1];
			Curr1.Order2_velocity[q2 - 1] = Order2_velocity[q2 - 1];
		}

	}

	shaking_index = (int)RandomRge((float)1.0, (float)(shaking_ratio * N_Order));

	for (int aa = 1; aa <= shaking_index && N_Machine != 1; aa++)
	{
		int a = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reshaking_machine:

		int b = (int)RandomRge((float)1.0, (float)(N_Machine + 1 - 0.00001));

		if (b == Curr1.M_select[a - 1])
			goto reshaking_machine;
		Curr1.M_selectval[a - 1] = M_selectval[a - 1] = (Curr1.M_selectval[a - 1] - (float)Curr1.M_select[a - 1]) + (float)b;
		Curr1.M_select[a - 1] = M_select[a - 1] = b;
	}
	for (int a = 1; a <= (N_Order - 1); a++)
	{
		if (Order2_seqval[a - 1] > Order2_seqval[a])
		{
			a = a;
		}
	}
reshake:

	shaking_index = (int)RandomRge((float)1.0, (float)(shaking_ratio * N_Order));

	for (int aa = 1; aa <= shaking_index && N_Vehicle != 1; aa++)
	{
		int a = (int)RandomRge((float)1.0, (float)(N_Order + 1 - 0.00001));
	reshaking_vehicle:

		int b = (int)RandomRge((float)1.0, (float)(N_Vehicle + 1 - 0.00001));

		if (b == Curr.V_select[a - 1])
			goto reshaking_vehicle;
		Curr1.V_selectval[a - 1] = (Curr1.V_selectval[a - 1] - (float)Curr1.V_select[a - 1]) + (float)b;
		Curr1.V_select[a - 1] = b;
	}
	for (int a = 1; a <= (N_Order - 1); a++)
	{
		if (Order2_seqval[a - 1] > Order2_seqval[a])
		{
			a = a;
		}
	}
	for (int a = 1; a <= N_Vehicle; a++)
	{
		tmp_vehicle_size = 0;
		for (int b = 1; b <= N_Order; b++)
		{
			if (Curr1.V_select[b - 1] == a)
				tmp_vehicle_size += order_size[b - 1];

		}
		if (tmp_vehicle_size > vehicle_size)
		{
			for (int b = 1; b <= N_Order; b++)
			{
				Curr1.V_selectval[b - 1] = V_selectval[b - 1]; //**
				Curr1.V_select[b - 1] = V_select[b - 1]; //**
			}
			goto reshake;
		}
	}

	for (int i = 1; i <= N_Order; i++)
	{
		Order1_seq[i - 1] = Curr1.Order1_seq[i - 1];
		Order1_seqval[i - 1] = Curr1.Order1_seqval[i - 1];
		Order1_velocity[i - 1] = Curr1.Order1_velocity[i - 1];
		M_select[i - 1] = Curr1.M_select[i - 1];
		M_selectval[i - 1] = Curr1.M_selectval[i - 1];
		M_velocity[i - 1] = Curr1.M_velocity[i - 1];
		Order2_seq[i - 1] = Curr1.Order2_seq[i - 1];
		Order2_seqval[i - 1] = Curr1.Order2_seqval[i - 1];
		Order2_velocity[i - 1] = Curr1.Order2_velocity[i - 1];
		V_select[i - 1] = Curr1.V_select[i - 1];
		V_selectval[i - 1] = Curr1.V_selectval[i - 1];
		V_velocity[i - 1] = Curr1.V_velocity[i - 1];
	}

	CalObj();
	Curr1.obj = curr_obj;
	best_obj = (float)1000000000000.0;

}



void init_solu()
{

	//	int c;
	FILE* fpp1;
	fpp1 = fopen("solution.txt", "w");
	for (int bb = 1; bb <= N_Particle; bb++)
	{
		random_initsol();
		CalObj();
		Curr.obj = curr_obj;
		best_obj = curr_obj;
		for (int b = 1; b <= N_Order; b++)
		{
			Best1.M_select[b - 1] = Curr1.M_select[b - 1] = Curr.M_select[b - 1];
			Best1.V_select[b - 1] = Curr1.V_select[b - 1] = Curr.V_select[b - 1];
			Best1.M_selectval[b - 1] = Curr1.M_selectval[b - 1] = Curr.M_selectval[b - 1];
			Best1.V_selectval[b - 1] = Curr1.V_selectval[b - 1] = Curr.V_selectval[b - 1];
			Best1.Order1_seq[b - 1] = Curr1.Order1_seq[b - 1] = Curr.Order1_seq[b - 1];
			Best1.Order2_seq[b - 1] = Curr1.Order2_seq[b - 1] = Curr.Order2_seq[b - 1];
			Best1.Order1_seqval[b - 1] = Curr1.Order1_seqval[b - 1] = Curr.Order1_seqval[b - 1];
			Best1.Order2_seqval[b - 1] = Curr1.Order2_seqval[b - 1] = Curr.Order2_seqval[b - 1];
			Best1.M_velocity[b - 1] = Curr1.M_velocity[b - 1] = Curr.M_velocity[b - 1] = M_velocity[b - 1] = RandomRge((float)(-N_Machine - 1 + 0.00001) / (float)m_w_para, (float)(N_Machine + 1 - 0.00001) / (float)m_w_para);
			Best1.V_velocity[b - 1] = Curr1.V_velocity[b - 1] = Curr.V_velocity[b - 1] = V_velocity[b - 1] = RandomRge((float)(-N_Vehicle - 1 + 0.00001) / (float)m_w_para, (float)(N_Vehicle + 1 - 0.00001) / (float)m_w_para);
			Best1.Order1_velocity[b - 1] = Curr1.Order1_velocity[b - 1] = Curr.Order1_velocity[b - 1] = Order1_velocity[b - 1] = RandomRge((float)(-N_Order - 1 + 0.00001) / (float)order_para, (float)(N_Order + 1 - 0.00001) / (float)order_para);
			Best1.Order2_velocity[b - 1] = Curr1.Order2_velocity[b - 1] = Curr.Order2_velocity[b - 1] = Order2_velocity[b - 1] = RandomRge((float)(-N_Order - 1 + 0.00001) / (float)order_para, (float)(N_Order + 1 - 0.00001) / (float)order_para);

		}

		Curr1.obj = Curr.obj;
		Best1.obj = Curr.obj;
		fclose(fpp1);
		samea = bb;
		PB[samea - 1].obj = Curr1.obj;
		VNS1();

		for (int b = 1; b <= shak_iter; b++)
		{

			shaking();
			VNS1();
		}

		now_time = timing(start1);
		PB[bb - 1].obj = PSO[bb - 1].obj = Best1.obj;

		for (int a = 1; a <= N_Order; a++)
		{
			PB[bb - 1].M_select[a - 1] = PSO[bb - 1].M_select[a - 1] = Best1.M_select[a - 1];
			PB[bb - 1].V_select[a - 1] = PSO[bb - 1].V_select[a - 1] = Best1.V_select[a - 1];
			PB[bb - 1].Order1_seq[a - 1] = PSO[bb - 1].Order1_seq[a - 1] = Best1.Order1_seq[a - 1];
			PB[bb - 1].Order2_seq[a - 1] = PSO[bb - 1].Order2_seq[a - 1] = Best1.Order2_seq[a - 1];
			PB[bb - 1].M_selectval[a - 1] = PSO[bb - 1].M_selectval[a - 1] = Best1.M_selectval[a - 1];
			PB[bb - 1].V_selectval[a - 1] = PSO[bb - 1].V_selectval[a - 1] = Best1.V_selectval[a - 1];
			PB[bb - 1].Order1_seqval[a - 1] = PSO[bb - 1].Order1_seqval[a - 1] = Best1.Order1_seqval[a - 1];
			PB[bb - 1].Order2_seqval[a - 1] = PSO[bb - 1].Order2_seqval[a - 1] = Best1.Order2_seqval[a - 1];
			PB[bb - 1].M_velocity[a - 1] = PSO[bb - 1].M_velocity[a - 1] = Best1.M_velocity[a - 1];
			PB[bb - 1].V_velocity[a - 1] = PSO[bb - 1].V_velocity[a - 1] = Best1.V_velocity[a - 1];
			PB[bb - 1].Order1_velocity[a - 1] = PSO[bb - 1].Order1_velocity[a - 1] = Best1.Order1_velocity[a - 1];
			PB[bb - 1].Order2_velocity[a - 1] = PSO[bb - 1].Order2_velocity[a - 1] = Best1.Order2_velocity[a - 1];
		}
	}
	for (int b = 1; b <= N_Particle; b++)
	{
		if (b == 1)
		{
			GB.obj = PB[b - 1].obj;
			for (int a = 1; a <= N_Order; a++)
			{
				GB.M_select[a - 1] = PB[b - 1].M_select[a - 1];
				GB.V_select[a - 1] = PB[b - 1].V_select[a - 1];
				GB.Order1_seq[a - 1] = PB[b - 1].Order1_seq[a - 1];
				GB.Order2_seq[a - 1] = PB[b - 1].Order2_seq[a - 1];
				GB.M_selectval[a - 1] = PB[b - 1].M_selectval[a - 1];
				GB.V_selectval[a - 1] = PB[b - 1].V_selectval[a - 1];
				GB.Order1_seqval[a - 1] = PB[b - 1].Order1_seqval[a - 1];
				GB.Order2_seqval[a - 1] = PB[b - 1].Order2_seqval[a - 1];
			}
		}
		else if (PB[b - 1].obj < GB.obj)
		{
			GB.obj = PB[b - 1].obj;
			for (int a = 1; a <= N_Order; a++)
			{
				GB.M_select[a - 1] = PB[b - 1].M_select[a - 1];
				GB.V_select[a - 1] = PB[b - 1].V_select[a - 1];
				GB.Order1_seq[a - 1] = PB[b - 1].Order1_seq[a - 1];
				GB.Order2_seq[a - 1] = PB[b - 1].Order2_seq[a - 1];
				GB.M_selectval[a - 1] = PB[b - 1].M_selectval[a - 1];
				GB.V_selectval[a - 1] = PB[b - 1].V_selectval[a - 1];
				GB.Order1_seqval[a - 1] = PB[b - 1].Order1_seqval[a - 1];
				GB.Order2_seqval[a - 1] = PB[b - 1].Order2_seqval[a - 1];
			}
		}
	}
	/*b = b + 1;
	fpp1 = fopen("solution.txt", "w");
	fprintf(fpp1, "GB.obj=%f\n", GB.obj);
	for (a = 1; a <= N_Particle; a++)
	{
	fprintf(fpp1, "particle %d obj=%f\n", a, PSO[a - 1].obj);
	for (b = 1; b <= N_Order; b++)
	{
	fprintf(fpp1, "%7.2f %3d %7.2f %7.2f %3d %7.2f %7.2f %3d %7.2f %7.2f %3d %7.2f\n", PSO[a - 1].Order1_seqval[b - 1], PSO[a - 1].Order1_seq[b - 1], PSO[a - 1].Order1_velocity[b - 1],
	PSO[a - 1].M_selectval[b - 1], PSO[a - 1].M_select[b - 1], PSO[a - 1].M_velocity[b - 1], PSO[a - 1].Order2_seqval[b - 1], PSO[a - 1].Order2_seq[b - 1], PSO[a - 1].Order2_velocity[b - 1],
	PSO[a - 1].V_selectval[b - 1], PSO[a - 1].V_select[b - 1], PSO[a - 1].V_velocity[b - 1]);
	}
	fprintf(fpp1, "\n\n");
	}
	fclose(fpp1);
	b = b + 1;*/



}

void ParticleSwarm()
{
	int  tmpindex, count_cycle, tmpindex1, tmpindex2, tmpindex3, tmpindex4, tmpindex5, tmp_vehicle_count;

	float tmpvalue;
	FILE* fppp1, * fppp2;

	tmp_vehicle_order = new int* [N_Vehicle];
	for (int i = 0; i < N_Vehicle; ++i)
	{
		tmp_vehicle_order[i] = new int[N_Order];//設置列
	}

	for (int a = 1; a <= N_Particle; a++)
	{
		count_cycle = 0;
		samea = a;
		for (int b = 1; b <= N_Order; b++)
		{
			if (iteration == 1)
			{
				PSO[a - 1].M_velocity[b - 1] = RandomRge((float)(-N_Machine - 1 + 0.00001) / (float)m_w_para, (float)(N_Machine + 1 - 0.00001) / (float)m_w_para);
				PSO[a - 1].V_velocity[b - 1] = RandomRge((float)(-N_Vehicle - 1 + 0.00001) / (float)m_w_para, (float)(N_Vehicle + 1 - 0.00001) / (float)m_w_para);
				PSO[a - 1].Order1_velocity[b - 1] = RandomRge((float)(-N_Order - 1 + 0.00001) / (float)order_para, (float)(N_Order + 1 - 0.00001) / (float)order_para);
				PSO[a - 1].Order2_velocity[b - 2] = RandomRge((float)(-N_Order - 1 + 0.00001) / (float)order_para, (float)(N_Order + 1 - 0.00001) / (float)order_para);
			}
			Test.M_velocity[b - 1] = pso_w * PSO[a - 1].M_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].M_selectval[b - 1] - PSO[a - 1].M_selectval[b - 1])
				+ pso_c2 * RandomRge(0.0, 1.0) * (GB.M_selectval[b - 1] - PSO[a - 1].M_selectval[b - 1]);
			Test.M_selectval[b - 1] = PSO[a - 1].M_selectval[b - 1] + Test.M_velocity[b - 1];

			if (Test.M_selectval[b - 1] > (float)N_Machine)
				Test.M_selectval[b - 1] = (float)(N_Machine + 1 - 0.02);
			else if (Test.M_selectval[b - 1] < (float)1.0)
				Test.M_selectval[b - 1] = (float)1.01;
			Test.M_select[b - 1] = (int)Test.M_selectval[b - 1];

			Test.V_velocity[b - 1] = pso_w * PSO[a - 1].V_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].V_selectval[b - 1] - PSO[a - 1].V_selectval[b - 1])
				+ pso_c2 * RandomRge(0.0, 1.0) * (GB.V_selectval[b - 1] - PSO[a - 1].V_selectval[b - 1]);
			Test.V_selectval[b - 1] = PSO[a - 1].V_selectval[b - 1] + Test.V_velocity[b - 1];

			if (Test.V_selectval[b - 1] > (float)N_Vehicle)
				Test.V_selectval[b - 1] = (float)(N_Vehicle + 1 - 0.02);
			else if (Test.V_selectval[b - 1] < (float)1.0)
				Test.V_selectval[b - 1] = (float)1.02;
			Test.V_select[b - 1] = (int)Test.V_selectval[b - 1];

			Test.Order1_velocity[b - 1] = pso_w * PSO[a - 1].Order1_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].Order1_seqval[b - 1] - PSO[a - 1].Order1_seqval[b - 1])
				+ pso_c2 * RandomRge(0.0, 1.0) * (GB.Order1_seqval[b - 1] - PSO[a - 1].Order1_seqval[b - 1]);
			Test.Order1_seqval[b - 1] = PSO[a - 1].Order1_seqval[b - 1] + Test.Order1_velocity[b - 1];


			if (Test.Order1_seqval[b - 1] > (float)N_Order)
				Test.Order1_seqval[b - 1] = (float)(N_Order + 1 - 0.02);
			else if (Test.Order1_seqval[b - 1] < (float)0.0)
				Test.Order1_seqval[b - 1] = (float)0.02;

			Test.Order1_seq[b - 1] = PSO[a - 1].Order1_seq[b - 1];

			Test.Order2_velocity[b - 1] = pso_w * PSO[a - 1].Order2_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].Order2_seqval[b - 1] - PSO[a - 1].Order2_seqval[b - 1])
				+ pso_c2 * RandomRge(0.0, 1.0) * (GB.Order2_seqval[b - 1] - PSO[a - 1].Order2_seqval[b - 1]);
			Test.Order2_seqval[b - 1] = PSO[a - 1].Order2_seqval[b - 1] + Test.Order2_velocity[b - 1];

			if (Test.Order2_seqval[b - 1] > (float)N_Order)
				Test.Order2_seqval[b - 1] = (float)(N_Order + 1 - 0.02);
			else if (Test.Order2_seqval[b - 1] < (float)0.0)
				Test.Order2_seqval[b - 1] = (float)0.02;

			Test.Order2_seq[b - 1] = PSO[a - 1].Order2_seq[b - 1];

		}

		for (int b = 1; b <= (N_Order - 1); b++)
		{
			for (int c = b + 1; c <= N_Order; c++)
			{
				if (Test.Order1_seqval[b - 1] > Test.Order1_seqval[c - 1])
				{
					tmpvalue = Test.Order1_seqval[b - 1];
					Test.Order1_seqval[b - 1] = Test.Order1_seqval[c - 1];
					Test.Order1_seqval[c - 1] = tmpvalue;
					tmpindex = Test.Order1_seq[b - 1];
					Test.Order1_seq[b - 1] = Test.Order1_seq[c - 1];
					Test.Order1_seq[c - 1] = tmpindex;
					tmpvalue = Test.Order1_velocity[b - 1];
					Test.Order1_velocity[b - 1] = Test.Order1_velocity[c - 1];
					Test.Order1_velocity[c - 1] = tmpvalue;

					tmpvalue = Test.M_selectval[b - 1];
					Test.M_selectval[b - 1] = Test.M_selectval[c - 1];
					Test.M_selectval[c - 1] = tmpvalue;
					tmpindex = Test.M_select[b - 1];
					Test.M_select[b - 1] = Test.M_select[c - 1];
					Test.M_select[c - 1] = tmpindex;
					tmpvalue = Test.M_velocity[b - 1];
					Test.M_velocity[b - 1] = Test.M_velocity[c - 1];
					Test.M_velocity[c - 1] = tmpvalue;
				}
				if (Test.Order2_seqval[b - 1] > Test.Order2_seqval[c - 1])
				{
					tmpvalue = Test.Order2_seqval[b - 1];
					Test.Order2_seqval[b - 1] = Test.Order2_seqval[c - 1];
					Test.Order2_seqval[c - 1] = tmpvalue;
					tmpindex = Test.Order2_seq[b - 1];
					Test.Order2_seq[b - 1] = Test.Order2_seq[c - 1];
					Test.Order2_seq[c - 1] = tmpindex;
					tmpvalue = Test.Order2_velocity[b - 1];
					Test.Order2_velocity[b - 1] = Test.Order2_velocity[c - 1];
					Test.Order2_velocity[c - 1] = tmpvalue;


					tmpvalue = Test.V_selectval[b - 1];
					Test.V_selectval[b - 1] = Test.V_selectval[c - 1];
					Test.V_selectval[c - 1] = tmpvalue;
					tmpindex = Test.V_select[b - 1];
					Test.V_select[b - 1] = Test.V_select[c - 1];
					Test.V_select[c - 1] = tmpindex;
					tmpvalue = Test.V_velocity[b - 1];
					Test.V_velocity[b - 1] = Test.V_velocity[c - 1];
					Test.V_velocity[c - 1] = tmpvalue;
				}
			}
		}


	evaluation:

		for (int iii = 1; iii <= N_Order; iii++)
		{
			tmp_vehicle_size1[iii - 1] = 0;
			tmp_vehicle_cap[iii - 1] = 0;
		}
		tmp_vehicle_count = 0;

		for (int iii = 1; iii <= N_Order; iii++)
		{
			tmpindex1 = Test.Order2_seq[iii - 1];
			tmpindex2 = Test.V_select[iii - 1];
			tmp_vehicle_size1[tmpindex2 - 1]++;
			tmpindex3 = tmp_vehicle_size1[tmpindex2 - 1];
			tmp_vehicle_cap[tmpindex2 - 1] += order_size[tmpindex1 - 1];
			tmp_vehicle_order[tmpindex2 - 1][tmpindex3 - 1] = tmpindex1;
		}

		for (int iii = 1; iii <= N_Vehicle; iii++)
		{

			if (tmp_vehicle_cap[iii - 1] > vehicle_size)
			{
				tmpindex1 = (int)RandomRge(1.0, (float)(tmp_vehicle_size1[iii - 1] + 1 - 0.00001));
				tmpindex2 = tmp_vehicle_order[iii - 1][tmpindex1 - 1];
				//  tmpindex3=order_size[tmpindex2-1];
				for (int jjj = 1; jjj <= N_Vehicle; jjj++)
				{
					tmpindex4 = tmp_vehicle_cap[jjj - 1] + order_size[tmpindex2 - 1];
					if (tmpindex4 <= vehicle_size)
					{
						tmp_vehicle_count++;
						tmp_vehicle[tmp_vehicle_count - 1] = jjj;
					}
				}
				tmpindex5 = (int)RandomRge(1.0, (float)(tmp_vehicle_count + 1 - 0.00001));
				for (int jjj = 1; jjj <= N_Order; jjj++)
				{
					if (Test.Order2_seq[jjj - 1] == tmpindex2)
					{
						Test.V_selectval[jjj - 1] = (Test.V_selectval[jjj - 1] - (float)Test.V_select[jjj - 1]) + tmp_vehicle[tmpindex5 - 1];
						Test.V_select[jjj - 1] = tmp_vehicle[tmpindex5 - 1];
						goto evaluation;
					}
				}
				break;
			}
		}

		for (int b = 1; b <= N_Order; b++)
		{
			Order1_seq[b - 1] = Test.Order1_seq[b - 1];
			Order1_seqval[b - 1] = Test.Order1_seqval[b - 1];
			Order1_velocity[b - 1] = Test.Order1_velocity[b - 1];
			M_select[b - 1] = Test.M_select[b - 1];
			M_selectval[b - 1] = Test.M_selectval[b - 1];
			M_velocity[b - 1] = Test.M_velocity[b - 1];
			Order2_seq[b - 1] = Test.Order2_seq[b - 1];
			Order2_seqval[b - 1] = Test.Order2_seqval[b - 1];
			Order2_velocity[b - 1] = Test.Order2_velocity[b - 1];
			V_select[b - 1] = Test.V_select[b - 1];
			V_selectval[b - 1] = Test.V_selectval[b - 1];
			V_velocity[b - 1] = Test.V_velocity[b - 1];

		}
		for (int i = 1; i <= N_Order; i++)
		{
			Curr.Order1_seq[i - 1] = Order1_seq[i - 1];	Curr.M_select[i - 1] = M_select[i - 1];
			Curr.Order2_seq[i - 1] = Order2_seq[i - 1];	Curr.V_select[i - 1] = V_select[i - 1];
			Curr.Order1_seqval[i - 1] = Order1_seqval[i - 1]; Curr.M_selectval[i - 1] = M_selectval[i - 1];
			Curr.Order2_seqval[i - 1] = Order2_seqval[i - 1]; Curr.V_selectval[i - 1] = V_selectval[i - 1];
			Curr.Order1_velocity[i - 1] = Order1_velocity[i - 1];  Curr.M_velocity[i - 1] = M_velocity[i - 1];
			Curr.Order2_velocity[i - 1] = Order2_velocity[i - 1];  Curr.V_velocity[i - 1] = V_velocity[i - 1];
		}

		CalObj();
		Curr.obj = curr_obj;
		best_obj = curr_obj;
		Curr1.obj = Curr.obj;
		Best1.obj = Curr.obj;
		
		for (int b = 1; b <= N_Order; b++)
		{
			Curr1.M_select[b - 1] = Curr.M_select[b - 1];
			Curr1.M_selectval[b - 1] = Curr.M_selectval[b - 1];
			Curr1.M_velocity[b - 1] = Curr.M_velocity[b - 1];
			Curr1.V_select[b - 1] = Curr.V_select[b - 1];
			Curr1.V_selectval[b - 1] = Curr.V_selectval[b - 1];
			Curr1.V_velocity[b - 1] = Curr.V_velocity[b - 1];
			Curr1.Order1_seq[b - 1] = Curr.Order1_seq[b - 1];
			Curr1.Order1_seqval[b - 1] = Curr.Order1_seqval[b - 1];
			Curr1.Order1_velocity[b - 1] = Curr.Order1_velocity[b - 1];
			Curr1.Order2_seq[b - 1] = Curr.Order2_seq[b - 1];
			Curr1.Order2_seqval[b - 1] = Curr.Order2_seqval[b - 1];
			Curr1.Order2_velocity[b - 1] = Curr.Order2_velocity[b - 1];

			Best1.M_select[b - 1] = Curr.M_select[b - 1];
			Best1.M_selectval[b - 1] = Curr.M_selectval[b - 1];
			Best1.M_velocity[b - 1] = Curr.M_velocity[b - 1];
			Best1.V_select[b - 1] = Curr.V_select[b - 1];
			Best1.V_selectval[b - 1] = Curr.V_selectval[b - 1];
			Best1.V_velocity[b - 1] = Curr.V_velocity[b - 1];
			Best1.Order1_seq[b - 1] = Curr.Order1_seq[b - 1];
			Best1.Order1_seqval[b - 1] = Curr.Order1_seqval[b - 1];
			Best1.Order1_velocity[b - 1] = Curr.Order1_velocity[b - 1];
			Best1.Order2_seq[b - 1] = Curr.Order2_seq[b - 1];
			Best1.Order2_seqval[b - 1] = Curr.Order2_seqval[b - 1];
			Best1.Order2_velocity[b - 1] = Curr.Order2_velocity[b - 1];
		}

		//Curr1.obj = Curr.obj;
		//Best1.obj = Curr.obj;

		VNS1();

		for (int b = 1; b <= shak_iter; b++)
		{
			shaking();
			VNS1();
		}

		now_time = timing(start1);
		DEV.obj = Best1.obj;
		for (int b = 1; b <= N_Order; b++)
		{
			
			DEV.M_select[b - 1] = Best1.M_select[b - 1];
			DEV.V_select[b - 1] = Best1.V_select[b - 1];
			DEV.Order1_seq[b - 1] = Best1.Order1_seq[b - 1];
			DEV.Order2_seq[b - 1] = Best1.Order2_seq[b - 1];
			DEV.M_selectval[b - 1] = Best1.M_selectval[b - 1];
			DEV.V_selectval[b - 1] = Best1.V_selectval[b - 1];
			DEV.Order1_seqval[b - 1] = Best1.Order1_seqval[b - 1];
			DEV.Order2_seqval[b - 1] = Best1.Order2_seqval[b - 1];
			DEV.M_velocity[b - 1] = Best1.M_velocity[b - 1];
			DEV.V_velocity[b - 1] = Best1.V_velocity[b - 1];
			DEV.Order1_velocity[b - 1] = Best1.Order1_velocity[b - 1];
			DEV.Order2_velocity[b - 1] = Best1.Order2_velocity[b - 1];
			}
		


			for (int b = 1; b <= N_Order; b++)
			{
				if (iteration == 1)
				{
					PSO[a - 1].M_velocity[b - 1] = RandomRge((float)(-N_Machine - 1 + 0.00001) / (float)m_w_para, (float)(N_Machine + 1 - 0.00001) / (float)m_w_para);
					PSO[a - 1].V_velocity[b - 1] = RandomRge((float)(-N_Vehicle - 1 + 0.00001) / (float)m_w_para, (float)(N_Vehicle + 1 - 0.00001) / (float)m_w_para);
					PSO[a - 1].Order1_velocity[b - 1] = RandomRge((float)(-N_Order - 1 + 0.00001) / (float)order_para, (float)(N_Order + 1 - 0.00001) / (float)order_para);
					PSO[a - 1].Order2_velocity[b - 2] = RandomRge((float)(-N_Order - 1 + 0.00001) / (float)order_para, (float)(N_Order + 1 - 0.00001) / (float)order_para);
				}
				Test.M_velocity[b - 1] = pso_w * PSO[a - 1].M_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].M_selectval[b - 1] - PSO[a - 1].M_selectval[b - 1])
					+ pso_c2 * RandomRge(0.0, 1.0) * (GB.M_selectval[b - 1] - PSO[a - 1].M_selectval[b - 1]);

				Test.M_selectval[b - 1] = PSO[a - 1].M_selectval[b - 1] + Test.M_velocity[b - 1];

				if (RandomRge(0.0, 1.0) < cr)  Test.M_selectval[b - 1] = DEV.M_selectval[b - 1] + RandomRge(0.0, 1.0) * (GB.M_selectval[b - 1] - PB[a - 1].M_selectval[b - 1]);
				else Test.M_selectval[b - 1] = PSO[a - 1].M_selectval[b - 1] + RandomRge(0.0, 1.0) * (GB.M_selectval[b - 1] - PB[a - 1].M_selectval[b - 1]);//交叉解


				if (Test.M_selectval[b - 1] > (float)N_Machine)
					Test.M_selectval[b - 1] = (float)(N_Machine + 1 - 0.02);

				else if (Test.M_selectval[b - 1] < (float)1.0)
					Test.M_selectval[b - 1] = (float)1.01;
				Test.M_select[b - 1] = (int)Test.M_selectval[b - 1];

				Test.V_velocity[b - 1] = pso_w * PSO[a - 1].V_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].V_selectval[b - 1] - PSO[a - 1].V_selectval[b - 1])
					+ pso_c2 * RandomRge(0.0, 1.0) * (GB.V_selectval[b - 1] - PSO[a - 1].V_selectval[b - 1]);
				Test.V_selectval[b - 1] = PSO[a - 1].V_selectval[b - 1] + Test.V_velocity[b - 1];

				if (RandomRge(0.0, 1.0) < cr)  Test.V_selectval[b - 1] = DEV.V_selectval[b - 1] + RandomRge(0.0, 1.0) * (GB.V_selectval[b - 1] - PB[a - 1].V_selectval[b - 1]);
				else Test.V_selectval[b - 1] = PSO[a - 1].V_selectval[b - 1] + RandomRge(0.0, 1.0) * (GB.V_selectval[b - 1] - PB[a - 1].V_selectval[b - 1]);//交叉解

				if (Test.V_selectval[b - 1] > (float)N_Vehicle)
					Test.V_selectval[b - 1] = (float)(N_Vehicle + 1 - 0.02);
				else if (Test.V_selectval[b - 1] < (float)1.0)
					Test.V_selectval[b - 1] = (float)1.02;
				Test.V_select[b - 1] = (int)Test.V_selectval[b - 1];

				Test.Order1_velocity[b - 1] = pso_w * PSO[a - 1].Order1_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].Order1_seqval[b - 1] - PSO[a - 1].Order1_seqval[b - 1])
					+ pso_c2 * RandomRge(0.0, 1.0) * (GB.Order1_seqval[b - 1] - PSO[a - 1].Order1_seqval[b - 1]);
				Test.Order1_seqval[b - 1] = PSO[a - 1].Order1_seqval[b - 1] + Test.Order1_velocity[b - 1];

				if (RandomRge(0.0, 1.0) < cr)  Test.Order1_seqval[b - 1] = DEV.Order1_seqval[b - 1] + RandomRge(0.0, 1.0) * (GB.Order1_seqval[b - 1] - PB[a - 1].Order1_seqval[b - 1]);
				else Test.Order1_seqval[b - 1] = PSO[a - 1].Order1_seqval[b - 1] + RandomRge(0.0, 1.0) * (GB.Order1_seqval[b - 1] - PB[a - 1].Order1_seqval[b - 1]);//交叉解


				if (Test.Order1_seqval[b - 1] > (float)N_Order)
					Test.Order1_seqval[b - 1] = (float)(N_Order + 1 - 0.02);
				else if (Test.Order1_seqval[b - 1] < (float)0.0)
					Test.Order1_seqval[b - 1] = (float)0.02;

				Test.Order1_seq[b - 1] = PSO[a - 1].Order1_seq[b - 1];

				Test.Order2_velocity[b - 1] = pso_w * PSO[a - 1].Order2_velocity[b - 1] + pso_c1[a - 1] * RandomRge(0.0, 1.0) * (PB[a - 1].Order2_seqval[b - 1] - PSO[a - 1].Order2_seqval[b - 1])
					+ pso_c2 * RandomRge(0.0, 1.0) * (GB.Order2_seqval[b - 1] - PSO[a - 1].Order2_seqval[b - 1]);
				Test.Order2_seqval[b - 1] = PSO[a - 1].Order2_seqval[b - 1] + Test.Order2_velocity[b - 1];

				if (RandomRge(0.0, 1.0) < cr)  Test.Order2_seqval[b - 1] = DEV.Order2_seqval[b - 1] + RandomRge(0.0, 1.0) * (GB.Order2_seqval[b - 1] - PB[a - 1].Order2_seqval[b - 1]);
				else Test.Order2_seqval[b - 1] = PSO[a - 1].Order2_seqval[b - 1] + RandomRge(0.0, 1.0) * (GB.Order2_seqval[b - 1] - PB[a - 1].Order2_seqval[b - 1]);//交叉解

				if (Test.Order2_seqval[b - 1] > (float)N_Order)
					Test.Order2_seqval[b - 1] = (float)(N_Order + 1 - 0.02);
				else if (Test.Order2_seqval[b - 1] < (float)0.0)
					Test.Order2_seqval[b - 1] = (float)0.02;

				Test.Order2_seq[b - 1] = PSO[a - 1].Order2_seq[b - 1];

			}

			for (int b = 1; b <= (N_Order - 1); b++)
			{
				for (int c = b + 1; c <= N_Order; c++)
				{
					if (Test.Order1_seqval[b - 1] > Test.Order1_seqval[c - 1])
					{
						tmpvalue = Test.Order1_seqval[b - 1];
						Test.Order1_seqval[b - 1] = Test.Order1_seqval[c - 1];
						Test.Order1_seqval[c - 1] = tmpvalue;
						tmpindex = Test.Order1_seq[b - 1];
						Test.Order1_seq[b - 1] = Test.Order1_seq[c - 1];
						Test.Order1_seq[c - 1] = tmpindex;
						tmpvalue = Test.Order1_velocity[b - 1];
						Test.Order1_velocity[b - 1] = Test.Order1_velocity[c - 1];
						Test.Order1_velocity[c - 1] = tmpvalue;

						tmpvalue = Test.M_selectval[b - 1];
						Test.M_selectval[b - 1] = Test.M_selectval[c - 1];
						Test.M_selectval[c - 1] = tmpvalue;
						tmpindex = Test.M_select[b - 1];
						Test.M_select[b - 1] = Test.M_select[c - 1];
						Test.M_select[c - 1] = tmpindex;
						tmpvalue = Test.M_velocity[b - 1];
						Test.M_velocity[b - 1] = Test.M_velocity[c - 1];
						Test.M_velocity[c - 1] = tmpvalue;
					}
					if (Test.Order2_seqval[b - 1] > Test.Order2_seqval[c - 1])
					{
						tmpvalue = Test.Order2_seqval[b - 1];
						Test.Order2_seqval[b - 1] = Test.Order2_seqval[c - 1];
						Test.Order2_seqval[c - 1] = tmpvalue;
						tmpindex = Test.Order2_seq[b - 1];
						Test.Order2_seq[b - 1] = Test.Order2_seq[c - 1];
						Test.Order2_seq[c - 1] = tmpindex;
						tmpvalue = Test.Order2_velocity[b - 1];
						Test.Order2_velocity[b - 1] = Test.Order2_velocity[c - 1];
						Test.Order2_velocity[c - 1] = tmpvalue;


						tmpvalue = Test.V_selectval[b - 1];
						Test.V_selectval[b - 1] = Test.V_selectval[c - 1];
						Test.V_selectval[c - 1] = tmpvalue;
						tmpindex = Test.V_select[b - 1];
						Test.V_select[b - 1] = Test.V_select[c - 1];
						Test.V_select[c - 1] = tmpindex;
						tmpvalue = Test.V_velocity[b - 1];
						Test.V_velocity[b - 1] = Test.V_velocity[c - 1];
						Test.V_velocity[c - 1] = tmpvalue;
					}
				}
			}


		evaluation1:

			for (int iii = 1; iii <= N_Order; iii++)
			{
				tmp_vehicle_size1[iii - 1] = 0;
				tmp_vehicle_cap[iii - 1] = 0;
			}
			tmp_vehicle_count = 0;

			for (int iii = 1; iii <= N_Order; iii++)
			{
				tmpindex1 = Test.Order2_seq[iii - 1];
				tmpindex2 = Test.V_select[iii - 1];
				tmp_vehicle_size1[tmpindex2 - 1]++;
				tmpindex3 = tmp_vehicle_size1[tmpindex2 - 1];
				tmp_vehicle_cap[tmpindex2 - 1] += order_size[tmpindex1 - 1];
				tmp_vehicle_order[tmpindex2 - 1][tmpindex3 - 1] = tmpindex1;
			}

			for (int iii = 1; iii <= N_Vehicle; iii++)
			{

				if (tmp_vehicle_cap[iii - 1] > vehicle_size)
				{
					tmpindex1 = (int)RandomRge(1.0, (float)(tmp_vehicle_size1[iii - 1] + 1 - 0.00001));
					tmpindex2 = tmp_vehicle_order[iii - 1][tmpindex1 - 1];
					//  tmpindex3=order_size[tmpindex2-1];
					for (int jjj = 1; jjj <= N_Vehicle; jjj++)
					{
						tmpindex4 = tmp_vehicle_cap[jjj - 1] + order_size[tmpindex2 - 1];
						if (tmpindex4 <= vehicle_size)
						{
							tmp_vehicle_count++;
							tmp_vehicle[tmp_vehicle_count - 1] = jjj;
						}
					}
					tmpindex5 = (int)RandomRge(1.0, (float)(tmp_vehicle_count + 1 - 0.00001));
					for (int jjj = 1; jjj <= N_Order; jjj++)
					{
						if (Test.Order2_seq[jjj - 1] == tmpindex2)
						{
							Test.V_selectval[jjj - 1] = (Test.V_selectval[jjj - 1] - (float)Test.V_select[jjj - 1]) + tmp_vehicle[tmpindex5 - 1];
							Test.V_select[jjj - 1] = tmp_vehicle[tmpindex5 - 1];
							goto evaluation1;
						}
					}
					break;
				}
			}

			for (int b = 1; b <= N_Order; b++)
			{
				Order1_seq[b - 1] = Test.Order1_seq[b - 1];
				Order1_seqval[b - 1] = Test.Order1_seqval[b - 1];
				Order1_velocity[b - 1] = Test.Order1_velocity[b - 1];
				M_select[b - 1] = Test.M_select[b - 1];
				M_selectval[b - 1] = Test.M_selectval[b - 1];
				M_velocity[b - 1] = Test.M_velocity[b - 1];
				Order2_seq[b - 1] = Test.Order2_seq[b - 1];
				Order2_seqval[b - 1] = Test.Order2_seqval[b - 1];
				Order2_velocity[b - 1] = Test.Order2_velocity[b - 1];
				V_select[b - 1] = Test.V_select[b - 1];
				V_selectval[b - 1] = Test.V_selectval[b - 1];
				V_velocity[b - 1] = Test.V_velocity[b - 1];

			}
			for (int i = 1; i <= N_Order; i++)
			{
				Curr.Order1_seq[i - 1] = Order1_seq[i - 1];	Curr.M_select[i - 1] = M_select[i - 1];
				Curr.Order2_seq[i - 1] = Order2_seq[i - 1];	Curr.V_select[i - 1] = V_select[i - 1];
				Curr.Order1_seqval[i - 1] = Order1_seqval[i - 1]; Curr.M_selectval[i - 1] = M_selectval[i - 1];
				Curr.Order2_seqval[i - 1] = Order2_seqval[i - 1]; Curr.V_selectval[i - 1] = V_selectval[i - 1];
				Curr.Order1_velocity[i - 1] = Order1_velocity[i - 1];  Curr.M_velocity[i - 1] = M_velocity[i - 1];
				Curr.Order2_velocity[i - 1] = Order2_velocity[i - 1];  Curr.V_velocity[i - 1] = V_velocity[i - 1];
			}

			CalObj();
			Curr.obj = curr_obj;
			best_obj = curr_obj;
			Curr1.obj = Curr.obj;
			Best1.obj = Curr.obj;
			for (int b = 1; b <= N_Order; b++)
			{
				Curr1.M_select[b - 1] = Curr.M_select[b - 1];
				Curr1.M_selectval[b - 1] = Curr.M_selectval[b - 1];
				Curr1.M_velocity[b - 1] = Curr.M_velocity[b - 1];
				Curr1.V_select[b - 1] = Curr.V_select[b - 1];
				Curr1.V_selectval[b - 1] = Curr.V_selectval[b - 1];
				Curr1.V_velocity[b - 1] = Curr.V_velocity[b - 1];
				Curr1.Order1_seq[b - 1] = Curr.Order1_seq[b - 1];
				Curr1.Order1_seqval[b - 1] = Curr.Order1_seqval[b - 1];
				Curr1.Order1_velocity[b - 1] = Curr.Order1_velocity[b - 1];
				Curr1.Order2_seq[b - 1] = Curr.Order2_seq[b - 1];
				Curr1.Order2_seqval[b - 1] = Curr.Order2_seqval[b - 1];
				Curr1.Order2_velocity[b - 1] = Curr.Order2_velocity[b - 1];

				Best1.M_select[b - 1] = Curr.M_select[b - 1];
				Best1.M_selectval[b - 1] = Curr.M_selectval[b - 1];
				Best1.M_velocity[b - 1] = Curr.M_velocity[b - 1];
				Best1.V_select[b - 1] = Curr.V_select[b - 1];
				Best1.V_selectval[b - 1] = Curr.V_selectval[b - 1];
				Best1.V_velocity[b - 1] = Curr.V_velocity[b - 1];
				Best1.Order1_seq[b - 1] = Curr.Order1_seq[b - 1];
				Best1.Order1_seqval[b - 1] = Curr.Order1_seqval[b - 1];
				Best1.Order1_velocity[b - 1] = Curr.Order1_velocity[b - 1];
				Best1.Order2_seq[b - 1] = Curr.Order2_seq[b - 1];
				Best1.Order2_seqval[b - 1] = Curr.Order2_seqval[b - 1];
				Best1.Order2_velocity[b - 1] = Curr.Order2_velocity[b - 1];
			}

			//Curr1.obj = Curr.obj;
			//Best1.obj = Curr.obj;

			VNS1();

			for (int b = 1; b <= shak_iter; b++)
			{
				shaking();
				VNS1();
			}
			
			now_time = timing(start1);
			DEU.obj = Best1.obj;
			for (int b = 1; b <= N_Order; b++)
			{
				
				DEU.M_select[b - 1] = Best1.M_select[b - 1];
				DEU.V_select[b - 1] = Best1.V_select[b - 1];
				DEU.Order1_seq[b - 1] = Best1.Order1_seq[b - 1];
				DEU.Order2_seq[b - 1] = Best1.Order2_seq[b - 1];
				DEU.M_selectval[b - 1] = Best1.M_selectval[b - 1];
				DEU.V_selectval[b - 1] = Best1.V_selectval[b - 1];
				DEU.Order1_seqval[b - 1] = Best1.Order1_seqval[b - 1];
				DEU.Order2_seqval[b - 1] = Best1.Order2_seqval[b - 1];
				DEU.M_velocity[b - 1] = Best1.M_velocity[b - 1];
				DEU.V_velocity[b - 1] = Best1.V_velocity[b - 1];
				DEU.Order1_velocity[b - 1] = Best1.Order1_velocity[b - 1];
				DEU.Order2_velocity[b - 1] = Best1.Order2_velocity[b - 1];
				

			}
			


			 if (DEV.obj < PB[a - 1].obj && PB[a - 1].obj < DEU.obj)
			{
				PB[a - 1].obj = PSO[a - 1].obj = DEV.obj;
				for (int b = 1; b <= N_Order; b++)
				{
					PB[a - 1].M_select[b - 1] = PSO[a - 1].M_select[b - 1] = DEV.M_select[b - 1];
					PB[a - 1].V_select[b - 1] = PSO[a - 1].V_select[b - 1] = DEV.V_select[b - 1];
					PB[a - 1].Order1_seq[b - 1] = PSO[a - 1].Order1_seq[b - 1] = DEV.Order1_seq[b - 1];
					PB[a - 1].Order2_seq[b - 1] = PSO[a - 1].Order2_seq[b - 1] = DEV.Order2_seq[b - 1];
					PB[a - 1].M_selectval[b - 1] = PSO[a - 1].M_selectval[b - 1] = DEV.M_selectval[b - 1];
					PB[a - 1].V_selectval[b - 1] = PSO[a - 1].V_selectval[b - 1] = DEV.V_selectval[b - 1];
					PB[a - 1].Order1_seqval[b - 1] = PSO[a - 1].Order1_seqval[b - 1] = DEV.Order1_seqval[b - 1];
					PB[a - 1].Order2_seqval[b - 1] = PSO[a - 1].Order2_seqval[b - 1] = DEV.Order2_seqval[b - 1];
					PB[a - 1].M_velocity[b - 1] = PSO[a - 1].M_velocity[b - 1] = DEV.M_velocity[b - 1];
					PB[a - 1].V_velocity[b - 1] = PSO[a - 1].V_velocity[b - 1] = DEV.V_velocity[b - 1];
					PB[a - 1].Order1_velocity[b - 1] = PSO[a - 1].Order1_velocity[b - 1] = DEV.Order1_velocity[b - 1];
					PB[a - 1].Order2_velocity[b - 1] = PSO[a - 1].Order2_velocity[b - 1] = DEV.Order2_velocity[b - 1];
				}
			}
			else if (DEU.obj < PB[a - 1].obj && PB[a - 1].obj < DEV.obj)
			{
				PB[a - 1].obj = PSO[a - 1].obj = DEU.obj;
				for (int b = 1; b <= N_Order; b++)
				{
					PB[a - 1].M_select[b - 1] = PSO[a - 1].M_select[b - 1] = DEU.M_select[b - 1];
					PB[a - 1].V_select[b - 1] = PSO[a - 1].V_select[b - 1] = DEU.V_select[b - 1];
					PB[a - 1].Order1_seq[b - 1] = PSO[a - 1].Order1_seq[b - 1] = DEU.Order1_seq[b - 1];
					PB[a - 1].Order2_seq[b - 1] = PSO[a - 1].Order2_seq[b - 1] = DEU.Order2_seq[b - 1];
					PB[a - 1].M_selectval[b - 1] = PSO[a - 1].M_selectval[b - 1] = DEU.M_selectval[b - 1];
					PB[a - 1].V_selectval[b - 1] = PSO[a - 1].V_selectval[b - 1] = DEU.V_selectval[b - 1];
					PB[a - 1].Order1_seqval[b - 1] = PSO[a - 1].Order1_seqval[b - 1] = DEU.Order1_seqval[b - 1];
					PB[a - 1].Order2_seqval[b - 1] = PSO[a - 1].Order2_seqval[b - 1] = DEU.Order2_seqval[b - 1];
					PB[a - 1].M_velocity[b - 1] = PSO[a - 1].M_velocity[b - 1] = DEU.M_velocity[b - 1];
					PB[a - 1].V_velocity[b - 1] = PSO[a - 1].V_velocity[b - 1] = DEU.V_velocity[b - 1];
					PB[a - 1].Order1_velocity[b - 1] = PSO[a - 1].Order1_velocity[b - 1] = DEU.Order1_velocity[b - 1];
					PB[a - 1].Order2_velocity[b - 1] = PSO[a - 1].Order2_velocity[b - 1] = DEU.Order2_velocity[b - 1];
				}
			}
			else
			 {
				 PB[a - 1].obj = PSO[a - 1].obj;
				 for (int b = 1; b <= N_Order; b++)
				 { 
					 PB[a - 1].M_select[b - 1] = PSO[a - 1].M_select[b - 1] ;
					 PB[a - 1].V_select[b - 1] = PSO[a - 1].V_select[b - 1] ;
					 PB[a - 1].Order1_seq[b - 1] = PSO[a - 1].Order1_seq[b - 1] ;
					 PB[a - 1].Order2_seq[b - 1] = PSO[a - 1].Order2_seq[b - 1] ;
					 PB[a - 1].M_selectval[b - 1] = PSO[a - 1].M_selectval[b - 1] ;
					 PB[a - 1].V_selectval[b - 1] = PSO[a - 1].V_selectval[b - 1] ;
					 PB[a - 1].Order1_seqval[b - 1] = PSO[a - 1].Order1_seqval[b - 1] ;
					 PB[a - 1].Order2_seqval[b - 1] = PSO[a - 1].Order2_seqval[b - 1] ;
					 PB[a - 1].M_velocity[b - 1] = PSO[a - 1].M_velocity[b - 1] ;
					 PB[a - 1].V_velocity[b - 1] = PSO[a - 1].V_velocity[b - 1] ;
					 PB[a - 1].Order1_velocity[b - 1] = PSO[a - 1].Order1_velocity[b - 1] ;
					 PB[a - 1].Order2_velocity[b - 1] = PSO[a - 1].Order2_velocity[b - 1] ;
				 }
			 }

	}

	


	for (int b = 1; b <= N_Particle; b++)
	{
		if (b == 1)
		{
			GB.obj = PB[b - 1].obj;
			for (int a = 1; a <= N_Order; a++)
			{
				GB.M_select[a - 1] = PB[b - 1].M_select[a - 1];
				GB.V_select[a - 1] = PB[b - 1].V_select[a - 1];
				GB.Order1_seq[a - 1] = PB[b - 1].Order1_seq[a - 1];
				GB.Order2_seq[a - 1] = PB[b - 1].Order2_seq[a - 1];
				GB.M_selectval[a - 1] = PB[b - 1].M_selectval[a - 1];
				GB.V_selectval[a - 1] = PB[b - 1].V_selectval[a - 1];
				GB.Order1_seqval[a - 1] = PB[b - 1].Order1_seqval[a - 1];
				GB.Order2_seqval[a - 1] = PB[b - 1].Order2_seqval[a - 1];
			}
		}
		else if (PB[b - 1].obj < GB.obj)
		{
			GB.obj = PB[b - 1].obj;
			for (int a = 1; a <= N_Order; a++)
			{
				GB.M_select[a - 1] = PB[b - 1].M_select[a - 1];
				GB.V_select[a - 1] = PB[b - 1].V_select[a - 1];
				GB.Order1_seq[a - 1] = PB[b - 1].Order1_seq[a - 1];
				GB.Order2_seq[a - 1] = PB[b - 1].Order2_seq[a - 1];
				GB.M_selectval[a - 1] = PB[b - 1].M_selectval[a - 1];
				GB.V_selectval[a - 1] = PB[b - 1].V_selectval[a - 1];
				GB.Order1_seqval[a - 1] = PB[b - 1].Order1_seqval[a - 1];
				GB.Order2_seqval[a - 1] = PB[b - 1].Order2_seqval[a - 1];
			}
		}
	}
	/*b = b + 1;
	fppp1 = fopen("particlesolution.txt", "w");
	fprintf(fppp1, "GB.obj=%f\n", GB.obj);
	for (a = 1; a <= N_Particle; a++)
	{
	fprintf(fppp1, "particle %d obj=%f PB.obj=%f\n", a, PSO[a - 1].obj, PB[a - 1].obj);
	for (b = 1; b <= N_Order; b++)
	{
	fprintf(fppp1, "%7.2f %3d %7.2f %7.2f %3d %7.2f %7.2f %3d %7.2f %7.2f %3d %7.2f\n", PSO[a - 1].Order1_seqval[b - 1], PSO[a - 1].Order1_seq[b - 1], PSO[a - 1].Order1_velocity[b - 1],
	PSO[a - 1].M_selectval[b - 1], PSO[a - 1].M_select[b - 1], PSO[a - 1].M_velocity[b - 1], PSO[a - 1].Order2_seqval[b - 1], PSO[a - 1].Order2_seq[b - 1], PSO[a - 1].Order2_velocity[b - 1],
	PSO[a - 1].V_selectval[b - 1], PSO[a - 1].V_select[b - 1], PSO[a - 1].V_velocity[b - 1]);
	}
	fprintf(fppp1, "\n\n");
	}
	fclose(fppp1);
	b = b + 1;
	*/
}

void main()
{
	int* piter_old, giter_old;
	float* p_old_best, g_old_best;
	fp16 = fopen("vnstrend.txt", "w");
	SP = fopen("Time.txt", "w");
	GenData();
	iteration = 0;
	init_solu();
	

	fprintf(SP, "%.2f\t %.3f\n", GB.obj, timing(start1));

	p_old_best = new float[N_Particle];
	piter_old = new int[N_Particle];
	for (int j = 1; j <= N_Particle; j++)
	{
		p_old_best[j - 1] = PB[j - 1].obj;
		piter_old[j - 1] = 0;
	}
	g_old_best = GB.obj;
	giter_old = 0;
	tmp_vehicle_size1 = new int[N_Vehicle];

	tmp_vehicle_cap = new int[N_Vehicle];

	tmp_vehicle = new int[N_Vehicle];

	//tmp_vehicle_order = new int *[N_Vehicle];

	//j = (int)(N_Order / N_Vehicle) + 5;
	//for (i = 1; i <= N_Vehicle; i++)
	//tmp_vehicle_order[i - 1] = new int[j];

	while (now_time < end_time)
	{
		iteration = iteration + 1;

		printf("GB.obj=%f\n time=%f\n iteration=%d\n particle=%d\n", GB.obj, timing(start1), iteration, N_Particle);  //**

		fprintf(SP, "%.2f\t %.3f\n", GB.obj, timing(start1));																								  //	printf("\n\niteration=%d\n\n",iteration);
		ParticleSwarm();
		now_time = timing(start1);

		for (int j = 1; j <= N_Particle; j++)
		{
			if (PB[j - 1].obj < p_old_best[j - 1])
			{
				piter_old[j - 1] = iteration;
				p_old_best[j - 1] = PB[j - 1].obj;
			}

			else if ((iteration - piter_old[j - 1]) > 1)
			{
				if (pso_w > (float)0.1)
					pso_w = pso_w - (float)0.01;
				if (pso_c1[j - 1] <= (float)1.00)
					pso_c1[j - 1] = pso_c1[j - 1] + (float)0.03;
			}
		}

		if (GB.obj < g_old_best)
		{
			giter_old = iteration;
			g_old_best = GB.obj;
		}
		else if ((iteration - giter_old) > 2)
		{
			if (pso_w > (float)0.1)
				pso_w = pso_w - (float)0.01;
			if (pso_c2 <= (float)1.00)
				pso_c2 = pso_c2 + (float)0.05;
		}
	}
	fclose(fp16);
	printf("GB.obj=%f\n", GB.obj);

	for (int j = 1; j <= N_Order; j++)
		printf("%5d", GB.Order1_seq[j - 1]);
	printf("\n");
	for (int j = 1; j <= N_Order; j++)
		printf("%5d", GB.M_select[j - 1]);
	printf("\n");
	for (int j = 1; j <= N_Order; j++)
		printf("%5d", GB.Order2_seq[j - 1]);
	printf("\n");
	for (int j = 1; j <= N_Order; j++)
		printf("%5d", GB.V_select[j - 1]);
	printf("\n");

	fclose(SP);

	system("pause");

}