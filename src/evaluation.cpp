
#include "stdafx.h"
#include "evaluation.h"
#include "BSD_day_night_final.h"
#include "system/legacy_list.h"
#include <string.h>

char pattern;
//---------------------------------------------- 경고 평가를 위한 변수 선언
int warning;
int must;
bool may, over20, parking, opplane;
bool check_veh_out;

int n_frame_must_warning = 0;
int n_frame_may_warning = 0;
int n_frame_parking = 0;
int n_frame_opp_driving = 0;
int n_frame_no_vehicle = 0;

int n_frame_gave_warning_at_must_warning = 0;
int n_frame_gave_warning_at_may_warning = 0;
int n_frame_gave_warning_at_parking = 0;
int n_frame_gave_warning_at_opp_driving = 0;
int n_frame_gave_warning_at_no_vehicle = 0;

int n_frame_didnt_give_warning_at_must_warning = 0;
int n_frame_didnt_give_warning_at_may_warning = 0;
int n_frame_didnt_give_warning_at_parking = 0;
int n_frame_didnt_give_warning_at_opp_driving = 0;
int n_frame_didnt_give_warning_at_no_vehicle = 0;

int n_frame_slower_than_20 = 0;

int num_must_give_warning = 0;

int prev_must = 0;
bool prev_may;
int n_frame_must_warn_for_a_case = 0;
int n_frame_gave_warning_for_a_case = 0;

int counter_for_03 = 0;
int counter_for_10 = 0;

double temp_double;
int mis_detection = 0;
int false_detection = 0;
int false_start;
char temp_str_miss[256];
char temp_str_false[256];

bool false_warning_trigger_prev, false_warning_trigger_pres;

int key = 1;

FILE *detailFP;
FILE *resultFP;
FILE *resultLogFP;
//---------------------------------------------------------------------------

char Read_mark(int mark_width, int mark_height, int block_size, int im_width, int im_height, IMG_TYPE *img)
{
	int i, j, m, n;
	int b, w;
	char temp[256], *p, r;

	for(i = 0; i < mark_height; i++)
		for(j = 0; j < mark_width; j++)
		{
			b = 0;
			w = 0;

			for(m = i*block_size; m < i*block_size + block_size; m++)
				for(n = j*block_size; n < j*block_size + block_size; n++)
					if( img[m*im_width + n] > 240 )
						w++;
					else if( img[m*im_width + n] < 10 )
						b++;

			if( w > block_size*block_size * 8/10 )
				temp[i*mark_width + j] = '0';
			else if( b > block_size*block_size * 8/10 )
				temp[i*mark_width + j] = '1';
			else temp[i*mark_width + j] = '?';

		}

		temp[mark_height*mark_width] = 0;

		p = temp;
		i = mark_width - 1;
		r = 0;
		while(*p != 0)
		{
			if(*p == '1')
				r |= (1 << i);
			else if(*p == '0')
				r &= ~((char)(1 << i));
			else return 0;

			p++;
			i--;
		}

		return r;

}

void PatternAnalysis(char patt, int &must, bool &may, bool &over20, bool &parking, bool &opplane)
{
	if((patt & PATT_NOT_MUST) == PATT_NOT_MUST)
		must = 0;
	else if((patt & PATT_MUST1) == PATT_MUST1)
		must = 1;
	else if((patt & PATT_MUST2) == PATT_MUST2)
		must = 2;

	if(patt & PATT_MAY)
		may = true;
	else may = false;

	if(patt & PATT_20KMH)
		over20 = true;
	else over20 = false;

	if((patt & PATT_NOT_OPP) == PATT_NOT_OPP)
	{
		opplane = false;
		parking = false;
	}
	else if((patt & PATT_OPP_DRIVING) == PATT_OPP_DRIVING)
	{
		opplane = true;
		parking = false;
	}
	else if((patt & PATT_OPP_PARKING) == PATT_OPP_PARKING)
	{
		opplane = false;
		parking = true;
	}
}

#pragma warning(push)
#pragma warning(disable:4996)
void init_evaluation( char *filename, char *pathname )
{
	char temp_str[256];

	may = parking = opplane = over20 = false;
	check_veh_out = false;
	false_warning_trigger_prev = false_warning_trigger_pres = false;
	must = 0;
	mis_detection = false_detection = 0;
	n_frame_must_warning = n_frame_may_warning = 0;
	n_frame_parking = n_frame_opp_driving = 0;
	n_frame_no_vehicle = n_frame_gave_warning_at_must_warning = 0;
	n_frame_gave_warning_at_may_warning = n_frame_gave_warning_at_parking = 0;
	n_frame_gave_warning_at_opp_driving = n_frame_gave_warning_at_no_vehicle = 0;
	n_frame_didnt_give_warning_at_must_warning = n_frame_didnt_give_warning_at_may_warning = 0;
	n_frame_didnt_give_warning_at_parking = n_frame_didnt_give_warning_at_opp_driving = 0;
	n_frame_didnt_give_warning_at_no_vehicle = n_frame_slower_than_20 = 0;

	num_must_give_warning = 0;

	prev_must = 0;
	prev_may = false;
	n_frame_must_warn_for_a_case = n_frame_gave_warning_for_a_case = 0;

	counter_for_03 = counter_for_10 = 0;

	mis_detection = false_detection = 0;

#if !defined(RESULT_IN_ONEPATH)
	strcpy(temp_str, pathname);
	strcat(temp_str, "result_temp/");
#else
	strcpy(temp_str, RESULT_PATH);
#endif
	strcat(temp_str, filename);
	strcat(temp_str, "_result.txt");
	fopen_s(&resultFP, temp_str, "w");

#if !defined(RESULT_IN_ONEPATH)
	strcpy(temp_str, pathname);
	strcat(temp_str, "result_temp/");
#else
	strcpy(temp_str, RESULT_PATH);
#endif
	strcat(temp_str, filename);
	strcat(temp_str, "_detail.txt");
	fopen_s(&detailFP, temp_str, "w");

#if !defined(RESULT_IN_ONEPATH)	
	strcpy(temp_str, pathname);
	strcat(temp_str, "result_temp/");
#else
	strcpy(temp_str, RESULT_PATH);
#endif
	strcat(temp_str, "result_log.csv");
	fopen_s(&resultLogFP, temp_str, "a+");

	fseek(resultLogFP,0,SEEK_END);
	if( ftell(resultLogFP)   == 0)
	{
		fprintf(resultLogFP, "filename, total frame , must warning , misDetection, false detection , case frames , gave warning \n");
		fflush(resultLogFP);
	}
	false_start = 0;
	sprintf_s(temp_str_miss,256,"");
	sprintf_s(temp_str_false,256,"");
	
}

void fin_evaluation(char *filename )
{
	fprintf(resultFP, "Statistics-----------------------------------------\n");
	fprintf(resultFP, "                     case frames   gave warning frames\n");
	fprintf(resultFP, " MUST                %6d     %16d\n", n_frame_must_warning, n_frame_gave_warning_at_must_warning);
	fprintf(resultFP, " MAY                 %6d     %16d\n", n_frame_may_warning, n_frame_gave_warning_at_may_warning);
	fprintf(resultFP, " Opp. lane-Parking   %6d     %16d\n", n_frame_parking, n_frame_gave_warning_at_parking); 
	fprintf(resultFP, " Opp. lane-Driving   %6d     %16d\n", n_frame_opp_driving, n_frame_gave_warning_at_opp_driving); 
	fprintf(resultFP, " No vehicle          %6d     %16d\n\n", n_frame_no_vehicle, n_frame_gave_warning_at_no_vehicle); 
	fprintf(resultFP, " Number of frames slower than 20km/h : %d\n\n", n_frame_slower_than_20);

	fprintf(resultFP, " Didn't warn at Must          %6d\n", n_frame_didnt_give_warning_at_must_warning);
	fprintf(resultFP, " Didn't warn at May           %6d\n", n_frame_didnt_give_warning_at_may_warning);
	fprintf(resultFP, " Didn't warn at Opp. parking  %6d\n", n_frame_didnt_give_warning_at_parking);
	fprintf(resultFP, " Didn't warn at Opp. driving  %6d\n", n_frame_didnt_give_warning_at_opp_driving);
	fprintf(resultFP, " Didn't warn at No vehicle    %6d\n\n", n_frame_didnt_give_warning_at_no_vehicle);

	fprintf(resultFP, " misDetection : %d / %d\n", mis_detection, num_must_give_warning);
	fprintf(resultFP, " False Detection : %d\n", false_detection);

	// Name , Frame  , Total , Miss , Case , Warning 
	int nTotalFrame = n_frame_must_warning + n_frame_may_warning + n_frame_parking +n_frame_opp_driving+n_frame_no_vehicle+n_frame_slower_than_20;
	fprintf(resultLogFP , "%s , %d , %d , %d , %d , %d , %d , %s , %s\n",filename , nTotalFrame , num_must_give_warning ,mis_detection ,false_detection ,n_frame_must_warning, n_frame_gave_warning_at_must_warning, temp_str_miss, temp_str_false);

	fclose(resultFP);
	fclose(detailFP);
	fclose(resultLogFP);
}


void evaluation(int frame_num, IMG_TYPE *img, int im_width, int im_height, int warning)
{
	char temp_str[256];
	char temp_str2[256];	

	pattern = Read_mark(MARK_width, MARK_height, PATTERN_BLOCK_SIZE, im_width, im_height, img);
	PatternAnalysis(pattern, must, may, over20, parking, opplane);

	sprintf_s(temp_str, 256, "%dth frame :", frame_num);

	if(over20)
	{
		if( !prev_must && must )	// MUST가 새로 시작 됨
		{
			counter_for_03 = FRAME_RATE * 3 / 10;	//0.3초 동안은 MUST 대신 강제로 MAY 설정
			num_must_give_warning++;
			n_frame_must_warn_for_a_case = 0;
			n_frame_gave_warning_for_a_case = 0;
		}
		else if( prev_must && must && prev_must != must )	// MUST의 상태가 바뀜
		{
			num_must_give_warning++;
			temp_double = (double)n_frame_gave_warning_for_a_case / n_frame_must_warn_for_a_case;
			printf("\t MUST changed  %4.2f\n", temp_double);
			n_frame_must_warn_for_a_case = 0;
			n_frame_gave_warning_for_a_case = 0;

			if(temp_double < MIS_DETECTION_LIMIT)
			{
				mis_detection++;
				strcat(temp_str, " misdetection added ");
				fprintf(resultFP,"%dth frame miss\n",frame_num);
				sprintf_s(temp_str2,256,"%d-",frame_num);
				strcat(temp_str_miss,temp_str2);
				
			}
		}
		else if( prev_must && !must )	// MUST가 끝남
		{
			
			temp_double = (double)n_frame_gave_warning_for_a_case / n_frame_must_warn_for_a_case;
			//counter_for_10 = 10;

			n_frame_must_warn_for_a_case = 0;
			n_frame_gave_warning_for_a_case = 0;

			if(temp_double < MIS_DETECTION_LIMIT)
			{
				mis_detection++;
				strcat(temp_str, " misdetection added ");
				fprintf(resultFP,"%dth frame miss\n",frame_num);
				sprintf_s(temp_str2,256,"%d-",frame_num);
				strcat(temp_str_miss,temp_str2);
			}
		}

		if( prev_must && !must && may )
			check_veh_out = true;

		if( check_veh_out && prev_may && !may )// MAY를 1초 동안 유지
			counter_for_10 = FRAME_RATE;

		prev_must = must;
		prev_may = may;

		if( counter_for_03 )
		{
			counter_for_03--;
			//printf(" set MAY\n");
			must = 0;
			may = true;
		}

		if( counter_for_10 )
		{
			//printf(" set MAY\n");
			counter_for_10--;
			may = true;
		}

		if( must ) 
		{
			n_frame_must_warn_for_a_case++;
			n_frame_must_warning++;
		}
		else if( may ) n_frame_may_warning++;
		else if(parking) n_frame_parking++;
		else if(opplane) n_frame_opp_driving++;
		else n_frame_no_vehicle++;

		false_warning_trigger_prev = false_warning_trigger_pres;

		strcat(temp_str, warning?"O":"X");

		if( ((must || may || parking || opplane) && warning) ||	(!(must || may || parking || opplane) && !warning) )
			false_warning_trigger_pres = false;

		if( false_warning_trigger_prev && !false_warning_trigger_pres )	//falling edge
		{
			false_detection++;
			false_warning_trigger_prev = false;
			false_start = 0;
		}

		if( warning )
		{
			if( must )
			{
				n_frame_gave_warning_at_must_warning++;
				n_frame_gave_warning_for_a_case++;
			}
			else if( may )
			{
				n_frame_gave_warning_at_may_warning++;
			}
			else if(parking)
			{
				n_frame_gave_warning_at_parking++;
				//strcat(temp_str, " - false(parking)");
			}
			else if(opplane)
			{
				n_frame_gave_warning_at_opp_driving++;
			}
			else
			{
				n_frame_gave_warning_at_no_vehicle++;
				strcat(temp_str, " - false detection");
				if(false_start == 0){
					fprintf(resultFP,"%dth frame false\n",frame_num);
					sprintf_s(temp_str2,256,"%d-",frame_num);
					strcat(temp_str_false,temp_str2);
				}
				false_start++;

				false_warning_trigger_pres = true;		//rising edge
			}
		}
		else
		{
			if( must )
			{
				n_frame_didnt_give_warning_at_must_warning++;
				strcat(temp_str, " - mis detection");
			}
			else if( may )
			{
				n_frame_didnt_give_warning_at_may_warning++;
			}
			else if(parking)
			{
				n_frame_didnt_give_warning_at_parking++;
			}
			else if(opplane)
			{
				n_frame_didnt_give_warning_at_opp_driving++;
			}
			else
			{
				n_frame_didnt_give_warning_at_no_vehicle++;
			}
		}

		false_warning_trigger_prev = false_warning_trigger_pres;
	}
	else
	{
		n_frame_slower_than_20++;
		strcat(temp_str, "slower than 20km/h");
	}


	//printf("%s\n", temp_str);
	//printf("MUST %d MAY %d over20 %d parking %d opplane %d - warning %d\n", must, may, over20, parking, opplane,warning);
	//printf("%d %d\n\n", counter_for_03, counter_for_10);
	fprintf(detailFP, "%s\n", temp_str);

}
#pragma warning(pop)
