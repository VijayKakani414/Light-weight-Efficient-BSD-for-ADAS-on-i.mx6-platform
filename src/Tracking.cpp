
#include "stdafx.h"
#include "Tracking.h"
#include "KF.h"

#include <math.h>
#include <stdio.h>


void init_Tracking(TrackingInfo *objs)
{
	int i, j, k, l;

	for(i = 0; i < MAX_BLOB; i++)
	{
		objs[i].history_pointer = 0;
		objs[i].tracking_false_counter = 0;
		objs[i].tracking_success_counter = 0;
		objs[i].hasdata = 0;
		objs[i].weight_tracking_counter = 0;

		for(j = 0; j < NUM_TRACKING_FRAMES; j++)
		{
			objs[i].History[j].x_pos = new double *[N_STATE];
			objs[i].History[j].P_pos = new double *[N_STATE];

			for(k = 0; k < N_STATE; k++)
			{
				objs[i].History[j].x_pos[k] = new double[1];
				objs[i].History[j].x_pos[k][0] = 0;

				objs[i].History[j].P_pos[k] = new double[N_STATE];

				for(l = 0; l < N_STATE; l++)
					objs[i].History[j].P_pos[k][l] = (k == l ? 1000 : 0);
			}
		}

#if defined(NIGHT_HEURI_TRACK)
		objs[i].vx = 0;
		objs[i].vy = 0;
		objs[i].vxsum = 0;
		objs[i].x = 0;
		objs[i].y = 0;
		objs[i].tx = objs[i].ty = 0;
		objs[i].L = L1;
		objs[i].M = M1;
		objs[i].m_cnt = 0;
		objs[i].identity = ID0;
		objs[i].blobidx = -1;
		objs[i].pairChk_h = objs[i].pairChk_v = 0;
		objs[i].pairIdx_h = objs[i].pairIdx_v = -1;
		objs[i].pairDistance_h = 0;
		objs[i].pairDistance_v = 0;
		objs[i].pairType_h = NONE;
		objs[i].pairType_v = NONE;
		objs[i].closeDist_h = 0;
		objs[i].closeDist_v = 0;
		objs[i].closeIdx_h = -1;
		objs[i].closeIdx_v = -1;
		objs[i].ignite_counter = 0;
		objs[i].tvx = NOT_FOUND;
		objs[i].tvy = NOT_FOUND;
		for(j = 0; j < NIGHT_MAX_HISTORY; j++)
		{
			objs[i].txh[j] = NOT_FOUND;
			objs[i].tyh[j] = NOT_FOUND;
		}
		objs[i].HLdirection = NONE;
		objs[i].pair_cnt = 0;
		objs[i].initialx = 0;
		objs[i].initialy = 0;
		objs[i].by_count = 0;
		
		objs[i].ID9_Count = 0;
		objs[i].ID9_ty = 0;
#endif

	}
}

void fin_Tracking(TrackingInfo *objs)
{
	int i, j, k;

	for(i = 0; i < MAX_BLOB; i++)
	{
		for(j = 0; j < NUM_TRACKING_FRAMES; j++)
		{
			for(k = 0; k < N_STATE; k++)
			{
				delete [] objs[i].History[j].x_pos[k];
				delete [] objs[i].History[j].P_pos[k];
			}

			delete [] objs[i].History[j].x_pos;
			delete [] objs[i].History[j].P_pos;
		}

	}

}

#if defined(NIGHT_HEURI_TRACK)
void save_ObjectInfo2tracking4KF(TrackingInfo *objs, int idx, State x_pos, Matrix P_pos, int size, Flag flag_false)
{
	int i, j, k;

	objs[idx].hasdata = 1;

	k = objs[idx].history_pointer;

	for(i = 0; i < N_STATE; i++)
		objs[idx].History[k].x_pos[i][0] = x_pos[i][0];

	for(i = 0; i < N_STATE; i++)
		for(j = 0; j < N_STATE; j++)
			objs[idx].History[k].P_pos[i][j] = P_pos[i][j];


	objs[idx].History[k].size = size;

	objs[idx].tracking_success_counter++; //flag_false에 관계없이 증가하는 것으로 봐서 이것은 succession_counter인것 같다

	objs[idx].history_pointer = circular_move(k, 1, NUM_TRACKING_FRAMES); 

	//if(objs[idx].tracking_success_counter > NUM_TRACKING_FRAMES)
	//	objs[idx].tracking_success_counter = NUM_TRACKING_FRAMES;
	if(objs[idx].tracking_success_counter > 999)
		objs[idx].tracking_success_counter = 999;

	if( flag_false == 0 )
	{
		objs[idx].tracking_false_counter = 0;
		//Tracking Calibration
		//영역 따라 weight
	}
	else	// predict
	{
		objs[idx].tracking_false_counter++;
	}
	
}

#else //!defined(NIGHT_HEURI_TRACK)

void save_ObjectInfo2tracking4KF(TrackingInfo *objs, int idx, State x_pos, Matrix P_pos, int size, Flag flag_false)
{
	int i, j, k;

	objs[idx].hasdata = 1;

	k = objs[idx].history_pointer;

	for(i = 0; i < N_STATE; i++)
		objs[idx].History[k].x_pos[i][0] = x_pos[i][0];

	for(i = 0; i < N_STATE; i++)
		for(j = 0; j < N_STATE; j++)
			objs[idx].History[k].P_pos[i][j] = P_pos[i][j];


	objs[idx].History[k].size = size;

	objs[idx].tracking_success_counter++; //flag_false에 관계없이 증가하는 것으로 봐서 이것은 succession_counter인것 같다

	objs[idx].history_pointer = circular_move(k, 1, NUM_TRACKING_FRAMES); 

	if(objs[idx].tracking_success_counter > NUM_TRACKING_FRAMES)
		objs[idx].tracking_success_counter = NUM_TRACKING_FRAMES;

	if( flag_false == 0 )
	{
		objs[idx].tracking_false_counter = 0;
		//Tracking Calibration
		//영역 따라 weight
		if(x_pos[0][0] < 200)
			objs[idx].weight_tracking_counter++;
		else if(x_pos[0][0] < 400)
			objs[idx].weight_tracking_counter+=2;
		else 
			objs[idx].weight_tracking_counter+=3;
	}
	else	// predict
	{
		objs[idx].weight_tracking_counter++;

		objs[idx].tracking_false_counter++;
	}

	if(objs[idx].weight_tracking_counter > NUM_TRACKING_FRAMES*3)
		objs[idx].weight_tracking_counter = NUM_TRACKING_FRAMES*3;

}
#endif

ObjectInfo get_Historyinfo(TrackingInfo *objs, int idx)
{
	int k;

	k = circular_move(objs[idx].history_pointer, -1, NUM_TRACKING_FRAMES);

	return objs[idx].History[k];
}

void delete_ObjectInfo(TrackingInfo *objs, int idx)
{
	int i, j, k;

	objs[idx].tracking_false_counter = 0;	// 초기화
	objs[idx].hasdata = 0;				// 데이터가 없음을 표시
	objs[idx].history_pointer = 0;
	objs[idx].tracking_success_counter = 0;
	objs[idx].weight_tracking_counter = 0;

	for(i = 0; i < NUM_TRACKING_FRAMES; i++)		// 다 지워
	{
		for(j = 0; j < N_STATE; j++)
			objs[idx].History[i].x_pos[j][0] = 0;

		for(j = 0; j < N_STATE; j++)
			for(k = 0; k < N_STATE; k++)
				objs[idx].History[i].P_pos[j][k] = (j==k ? 9999 : 0);

		objs[idx].History[i].size = 0;
	}
#if defined(NIGHT_HEURI_TRACK)
	objs[idx].vx = 0;
	objs[idx].vy = 0;
	objs[idx].vxsum = 0;
	objs[idx].x = 0;
	objs[idx].y = 0;
	objs[idx].tx = objs[idx].ty = 0;
	objs[idx].L = L1;
	objs[idx].M = M1;
	objs[idx].m_cnt = 0;
	objs[idx].identity = ID0;
	objs[idx].blobidx = -1;
	objs[idx].pairChk_h = objs[idx].pairChk_v = 0;
	objs[idx].pairIdx_h = objs[idx].pairIdx_v = -1;
	objs[idx].pairDistance_h = 0;
	objs[idx].pairDistance_v = 0;
	objs[idx].pairType_h = NONE;
	objs[idx].pairType_v = NONE;
	objs[idx].closeDist_h = 0;
	objs[idx].closeDist_v = 0;
	objs[idx].closeIdx_h = -1;
	objs[idx].closeIdx_v = -1;
	objs[idx].ignite_counter = 0;
	objs[idx].tvx = NOT_FOUND;
	objs[idx].tvy = NOT_FOUND;
	for(j = 0; j < NIGHT_MAX_HISTORY; j++)
	{
		objs[idx].txh[j] = NOT_FOUND;
		objs[idx].tyh[j] = NOT_FOUND;
	}
	objs[idx].HLdirection = NONE;
	objs[idx].pair_cnt = 0;
	objs[idx].initialx = 0;
	objs[idx].initialy = 0;
	objs[idx].by_count = 0;
	objs[idx].ID9_Count = 0;
	objs[idx].ID9_ty = 0;
#endif
}


int Find_most_similar_one4KF(int num_objs, VEC2_U16 *loc, Coordinate *obj_size, Flag *flag_tracking, State x_neg, int size, int f)
{
	int i, D, d, index = -1;
	int x, y, r;

	x = (int)x_neg[0][0];
	y = (int)x_neg[1][0];
	//object를 검색하는 범위
	r = (int)sqrt(size / 3.141592f);

	for(i = 0; i < num_objs; i++)
		if(flag_tracking[i] == 0 && 
			x - r*f < loc[i].x && loc[i].x < x + r*f &&	 //이 부분 조건식을 바꾸면 DSP 버전에도 반영해야 함
			y - r*f < loc[i].y && loc[i].y < y + r*f && 
			obj_size[i]/2 < size && size < 2*obj_size[i] )
		{
			//if(obj_size[i] > 300 && loc[i].x < 400 && loc[i].y > 300 || obj_size[i] > 400)
			{
				index = i;
				break;
			}
		}

	if(index == -1)
		return index;

	for(i = index+1; i < num_objs; i++)
		if(flag_tracking[i] == 0 &&
			x - r*f < loc[i].x && loc[i].x < x + r*f &&	
			y - r*f < loc[i].y && loc[i].y < y + r*f)
			//if(obj_size[i] > 300 && loc[i].x < 400 && loc[i].y > 300 || obj_size[i] > 400)
			{
				D = (int)sqrt((double)(x-loc[index].x)*(x-loc[index].x) + (y-loc[index].y)*(y-loc[index].y));
				d = (int)sqrt((double)(x-loc[i].x)*(x-loc[i].x) + (y-loc[i].y)*(y-loc[i].y));
				if( D*D*obj_size[index] > d*d*obj_size[i] )
				index = i;
			}

	return index;
}

void Tracking_using_KF(int num_objs, VEC2_U16 *loc, Coordinate *size, TrackingInfo *objs, int n_track, Matrix F, Matrix Q, Matrix R, Matrix H)
{
	int x, y, r, n;
	int i, j, k, l, f=1;
	Flag flag_tracking[MAX_BLOB];
	ObjectInfo temp_ObjectInfo;
	State x_neg, x_pos;
	Matrix P_neg, P_pos, K;
	State meas;

	x_neg = new double *[N_STATE];
	x_pos = new double *[N_STATE];
	K = new double *[N_STATE];
	meas = new double *[N_MEAS];
	P_neg = new double *[N_STATE];
	P_pos = new double *[N_STATE];

	for(i = 0; i < N_STATE; i++)
	{
		x_neg[i] = new double[1];
		x_pos[i] = new double[1];
		K[i] = new double[N_MEAS];
		P_neg[i] = new double[N_STATE];
		P_pos[i] = new double[N_STATE];
	}
	for(i = 0; i < N_MEAS; i++)
		meas[i] = new double[1];
	/*
	for(i = 0; i < num_objs; i++)		// 이번frame의 obj들이 이전 frame에서 짝을 찾았는지를 나타내기 위한 flag
	if( size[i] < 100 )				// size에 대한 threshold. 크기가 100 이하인 경우는 tracking 하지 않음
	flag_tracking[i] = 1;
	else
	flag_tracking[i] = 0;
	*/
	for(i = 0; i < num_objs; i++)
		flag_tracking[i] = 0;

	for(i = 0; i < n_track; i++)			// tracking 중인 모든 obj에 대해 검색
	{
		if( objs[i].hasdata == 0 )		// 저장된 것이 없다면 다음 obj으로 넘어가자
			continue;

		temp_ObjectInfo = get_Historyinfo(objs, i);

		n = objs[i].tracking_success_counter; //LJP, 2011-04-13 지금 사용 안하는 코드

		calc_x_neg(F, temp_ObjectInfo.x_pos, Q, x_neg);
		calc_P_mtx_neg(F, temp_ObjectInfo.P_pos, Q, P_neg);
		/*for(k = 0; k < N_STATE; k++)
		{
		x_neg[k][0] = temp_ObjectInfo.x_pos[k][0]*n/(n+1) + x_neg[k][0]/(n+1);
		//x_neg[k][0] = temp_ObjectInfo.x_pos[k][0]*n/(n+1) + x_neg[k][0]/(n+1);
		}*/

		x = (int)x_neg[0][0];
		y = (int)x_neg[1][0];
		r = (int)sqrt(temp_ObjectInfo.size / 3.141592f);

		//		printf("%dth tracking est(%d,%d) - ", i, x, y);
		//		for(j=0; j < num_objs; j++)
		//			printf("(%d,%d)", x_loc[j], y_loc[j]);
		//		printf("\n");
		// 가장 비슷한 obj을 찾아냄
		j = Find_most_similar_one4KF(num_objs, loc, size, flag_tracking, x_neg, temp_ObjectInfo.size, 2); // KeyPoint_Night
		if(j != -1)
		{
			//			printf(" -> (%d, %d, %d)\n", x_loc[j], y_loc[j], r);
			// j 번째 obj을 tracking에 성공했음을 기록
			flag_tracking[j] = 1;
		}
		else
		{
			/*
			//비슷한 것을 찾지 못했을 경우에
			//범위를 2배로 늘려서 검색
			j = Find_most_similar_one4KF(num_objs, loc, size, flag_tracking, x_neg, temp_ObjectInfo.size, 2);
			if(j != -1)
			{
				//				printf(" -> (%d, %d, %d*2)\n", x_loc[j], y_loc[j], r);
				flag_tracking[j] = 1;
			}
			else
			{
				//				printf("couldn't find\n");
			}
			*/
		}


		if(j != -1)	// 어떻게든지 찾았다면 추가
		{
			meas[0][0] = loc[j].x;
			meas[1][0] = loc[j].y;
			meas[2][0] = loc[j].x - temp_ObjectInfo.x_pos[0][0];
			meas[3][0] = loc[j].y - temp_ObjectInfo.x_pos[1][0];
			calc_K_mtx(P_neg, H, R, K);
			calc_x_pos(x_neg, K, H, meas, x_pos);
			calc_P_mtx_pos(K, H, P_neg, P_pos);


			// 가속도의 미분에 대한 threshold 값을 적용. 가속도가 일정하다고 했지만 어느정도의 변화가 있는 것 처럼
			// 측정은 될 것. 노이즈 같은 경우는 좌우, 상하로 튀게 됨. 이 때의 가속도가 크게 나오는 것을 이용해서 걸러냄
			/*if( (abs(temp_ObjectInfo.x_pos[4][0] - x_pos[4][0]) > 3 && temp_ObjectInfo.x_pos[4][0]/temp_ObjectInfo.x_pos[4][0] != x_pos[4][0]/x_pos[4][0])
			|| (abs(temp_ObjectInfo.x_pos[5][0] - x_pos[5][0]) > 3 && temp_ObjectInfo.x_pos[5][0]/temp_ObjectInfo.x_pos[5][0] != x_pos[5][0]/x_pos[5][0]) )
			{
			//delete_ObjectInfo(objs, i);
			printf("acc. > 3 \n");
			x = (int)x_neg[0][0];
			y = (int)x_neg[1][0];

			if(objs[i].tracking_false_counter + 1 < TRACKING_FALSE_LIMIT &&  // 아직 봐줄만 하고
			MAY_LINE1(x,y) && MAY_LINE2(x,y) && MAY_LINE3(x,y) )		// 예상 위치가 MAY영역 안이면
			{
			save_ObjectInfo2tracking4KF(objs, i, x_neg, P_neg, temp_ObjectInfo.size, 1);	// predict
			}
			else												// 너무 오래 찾지 못하거나 예상 위치가 MAY영역 밖이면 지워
			{
			delete_ObjectInfo(objs, i);
			}
			}
			else*/ 


			//printf("Vx : %5.2f\n", x_pos[2][0]);
			save_ObjectInfo2tracking4KF(objs, i, x_pos, P_pos, size[j], 0);
#if defined(NIGHT_HEURI_TRACK)
			objs[i].blobidx = j; //save blob index 
#endif
		}
		else
		{
#if defined(NIGHT_HEURI_TRACK)
			objs[i].blobidx = -1;
#endif
			// 0 ~ num_obj-1 의 index중에 짝을 찾지 못했다면...
			x = (int)x_neg[0][0];
			y = (int)x_neg[1][0];
			//Tracking Calibration
			//false tracking을 몇 개까지 유지할 것인지
			//if(objs[i].tracking_false_counter + 1 < TRACKING_FALSE_LIMIT &&  // 아직 봐줄만 하고
			//	MAY_LINE1(x,y) && MAY_LINE2(x,y) && MAY_LINE3(x,y) && 0 < x && x < 640 && 0 < y && y < 480 )		// 예상 위치가 MAY영역 안이면
			if(objs[i].tracking_false_counter + 1 < TRACKING_FALSE_LIMIT &&  // 아직 봐줄만 하고
				0 < x && x < 640 && 0 < y && y < 480 )		// 예상 위치가 MAY영역 안이면
			{
				//printf("predict Vx : %5.2f\n", x_neg[2][0]);
				save_ObjectInfo2tracking4KF(objs, i, x_neg, P_neg, temp_ObjectInfo.size, 1);	// predict
			}
			else												// 너무 오래 찾지 못하거나 예상 위치가 MAY영역 밖이면 지워
			{
				//printf(" %dth tracking deleted\n", i);
				delete_ObjectInfo(objs, i);
			}
		}
	}

	// 이전 프레임에서 등장하지 않았던 새로운 obj이 나타 날 경우(새 tracking 시작하자)
	for(i = 0; i < num_objs; i++)
	{
		if( flag_tracking[i] == 0 && size[i] > 100 && loc[i].x != NOT_FOUND && loc[i].y != NOT_FOUND)	// 짝 못찾은 현재 프레임의 obj을 찾아서...
		{
			j = 0;
			while( objs[j].hasdata != 0 && j < n_track)	// 빈공간을 찾아서..
				j++;

			if(j < n_track)	// 찾았으면
			{
				delete_ObjectInfo(objs, j);
				objs[j].hasdata = 1;					// 여기는 주인있다고 표시하고,
#if defined(NIGHT_HEURI_TRACK)
				objs[j].blobidx = i; //save blob index 
#endif
				x_neg[0][0] = loc[i].x;
				x_neg[1][0] = loc[i].y;
				x_neg[2][0] = 0;
				x_neg[3][0] = 0;
				x_neg[4][0] = 0;
				x_neg[5][0] = 0;

				for(k = 0; k < N_STATE; k++)
					for(l = 0; l < N_STATE; l++)
						P_neg[k][l] = (k==l ? 9999 : 0);

				save_ObjectInfo2tracking4KF(objs, j, x_neg, P_neg, size[i], 0);
				objs[j].initialx = loc[i].x;
				objs[j].initialy = loc[i].y;
			}

		}
	}

	for(i = 0; i < N_STATE; i++)
	{
		delete [] x_neg[i];
		delete [] x_pos[i];
		delete [] K[i];
		delete [] P_pos[i];
		delete [] P_neg[i];
	}
	delete []x_neg;
	delete []x_pos;
	delete []P_neg;
	delete []P_pos;
	delete []K;

	for(i = 0; i < N_MEAS; i++)
		delete [] meas[i];
	delete []meas;

}


