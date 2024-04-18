
#include "stdafx.h"
#include "KF.h"
#include "Matrix.h"

int n_state, n_meas;

Matrix QN;
Matrix NQ1, NQ2;
Matrix QQ1, QQ2;
Matrix NN1, NN2, NN3;
Matrix N11, N12;
Matrix Q11, Q12;
Matrix eye;
State process_noise;

/**
	@brief	칼만 필터를 사용하기 위해 파라미터 초기화, 메모리 할당
	@return 없음
*/
void init_Kalman(
	int n,	///<KF state의 수 
	int q	///<KF measurement의 수 
)
{
	int i, j;

	n_state = n;
	n_meas = q;
	///매트릭스들 메모리 할당
	QN = new double *[q];
	for(i = 0; i < q; i++)
	{
		QN[i] = new double[n];
	}

	NQ1 = new double *[n];
	for(i = 0; i < n; i++)
	{
		NQ1[i] = new double[q];
	}

	NQ2 = new double *[n];
	for(i = 0; i < n; i++)
	{
		NQ2[i] = new double[q];
	}

	QQ1 = new double *[q];
	for(i = 0; i < q; i++)
	{
		QQ1[i] = new double[q];
	}

	QQ2 = new double *[q];
	for(i = 0; i < q; i++)
	{
		QQ2[i] = new double[q];
	}

	NN1 = new double *[n];
	for(i = 0; i < n; i++)
	{
		NN1[i] = new double[n];
	}

	NN2 = new double *[n];
	for(i = 0; i < n; i++)
	{
		NN2[i] = new double[n];
	}

	NN3 = new double *[n];
	for(i = 0; i < n; i++)
	{
		NN3[i] = new double[n];
	}

	process_noise = new double *[n];
	for(i = 0; i < n; i++)
	{
		process_noise[i] = new double[1];
	}

	N11 = new double *[n];
	for(i = 0; i < n; i++)
	{
		N11[i] = new double[1];
	}

	N12 = new double *[n];
	for(i = 0; i < n; i++)
	{
		N12[i] = new double[1];
	}

	Q11 = new double *[q];
	for(i = 0; i < q; i++)
	{
		Q11[i] = new double[1];
	}

	Q12 = new double *[q];
	for(i = 0; i < q; i++)
	{
		Q12[i] = new double[1];
	}
	///I 매트릭스 메모리 할당 및 초기화 
	eye = new double*[n];
	for(i = 0; i < n; i++)
	{
		eye[i] = new double[n];
		for(j = 0; j < n_state; j++)
		{
			eye[i][j] = 0;
		}

		eye[i][i] = 1;
	}
}
/**
	@brief	칼만 필터에 사용한 파라미터의 메모리 해제
	@return 없음
*/
void fin_Kalman(void)
{
	int i;

	for(i = 0; i < n_meas; i++)
	{
		delete []QN[i];
	}
	delete [] QN;

	for(i = 0; i < n_state; i++)
	{
		delete []NQ1[i];
	}
	delete [] NQ1;

	for(i = 0; i < n_state; i++)
	{
		delete []NQ2[i];
	}
	delete [] NQ2;

	for(i = 0; i < n_meas; i++)
	{
		delete []QQ1[i];
	}
	delete [] QQ1;

	for(i = 0; i < n_meas; i++)
	{
		delete []QQ2[i];
	}
	delete [] QQ2;

	for(i = 0; i < n_state; i++)
	{
		delete []NN1[i];
	}
	delete [] NN1;

	for(i = 0; i < n_state; i++)
	{
		delete []NN2[i];
	}
	delete [] NN2;

	for(i = 0; i < n_state; i++)
	{
		delete []NN3[i];
	}
	delete [] NN3;

	for(i = 0; i < n_state; i++)
	{
		delete []process_noise[i];
	}
	delete [] process_noise;

	for(i = 0; i < n_state; i++)
	{
		delete []N11[i];
	}
	delete []N11;

	for(i = 0; i < n_state; i++)
	{
		delete []N12[i];
	}
	delete [] N12;

	for(i = 0; i < n_meas; i++)
	{
		delete []Q11[i];
	}
	delete [] Q11;

	for(i = 0; i < n_meas; i++)
	{
		delete []Q12[i];
	}
	delete [] Q12;

	for(i = 0; i < n_state; i++)
	{
		delete []eye[i];
	}
	delete [] eye;
}

void calc_P_mtx_neg(Matrix F, Matrix P_pos_prev, Matrix Q, Matrix P_neg)
{
	mtx_trans(F, n_state, n_state, NN1);
	mtx_mul(F, n_state, n_state, P_pos_prev, n_state, n_state, NN2);
	mtx_mul(NN2, n_state, n_state, NN1, n_state, n_state, NN3);
	mtx_add(NN3, Q, n_state, n_state);

	mtx_subst(P_neg, NN3, n_state, n_state);
}

void calc_P_mtx_pos(Matrix K, Matrix H, Matrix P_neg, Matrix P_pos)
{
	mtx_subst(NN1, eye, n_state, n_state);
	mtx_mul(K, n_state, n_meas, H, n_meas, n_state, NN2);
	mtx_sub(NN1, NN2, n_state, n_state);
	mtx_mul(NN1, n_state, n_state, P_neg, n_state, n_state, P_pos);
}

void calc_K_mtx(Matrix P_neg, Matrix H, Matrix R, Matrix K)
{
	mtx_trans(H, n_meas, n_state, NQ1);

	mtx_mul(P_neg, n_state, n_state, NQ1, n_state, n_meas, NQ2);
	mtx_mul(H, n_meas, n_state, P_neg, n_state, n_state, QN);
	mtx_mul(QN, n_meas, n_state, NQ1, n_state, n_meas, QQ1);
	mtx_add(QQ1, R, n_meas, n_meas);
	mtx_inv(QQ1, n_meas, n_meas, QQ2);
	mtx_mul(NQ2, n_state, n_meas, QQ2, n_meas, n_meas, K);
}
/**
	@brief	x(-) 계산, x(-)는 이전 프레임의 위치와 state machine(A)와 conditional input(B)로부터 계산
	x(k)(-) = Ax(k-1)(-) + Bu(k-1)
*/
void calc_x_neg(Matrix F, State x_pos, Matrix Q, State x_neg)
{
	int i;

	for(i = 0; i < n_state; i++)
	{
		process_noise[i][0] = 0;
	}

	mtx_mul(F, n_state, n_state, x_pos, n_state, 1, x_neg);
	mtx_add(x_neg, process_noise, n_state, 1);
}

void calc_x_pos(State x_neg, Matrix K, Matrix H, State y, State x_pos)
{
	mtx_subst(N11, x_neg, n_state, 1);
	mtx_subst(Q11, y, n_meas, 1);

	mtx_mul(H, n_meas, n_state, x_neg, n_state, 1, Q12);
	mtx_sub(Q11, Q12, n_meas, 1);

	mtx_mul(K, n_state, n_meas, Q11, n_meas, 1, N12);
	mtx_add(N11, N12, n_state, 1);

	mtx_subst(x_pos, N11, n_state, 1);
}
