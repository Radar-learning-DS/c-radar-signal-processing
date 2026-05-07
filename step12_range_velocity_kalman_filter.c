#include <stdio.h>

typedef struct {
	float x[2]; // state : [range, velocity]

	float P[2][2]; // error covariance
	float F[2][2]; // state transition matrix
	float H[2]; //measurement matrix
	float Q[2][2]; // process noise
	float R; //measurement noise

	float K[2]; // Kalman gain

} KalmanFilter2D;

void initKalman(KalmanFilter2D* kf, float initRange, float initVelocity, float dt) {
	kf->x[0] = initRange;
	kf->x[1] = initVelocity;

	kf->F[0][0] = 1.0f;
	kf->F[0][1] = dt;
	kf->F[1][0] = 0.0f;
	kf->F[1][1] = 1.0f;

	kf->H[0] = 1.0f;
	kf->H[1] = 0.0f;

	kf->P[0][0] = 1.0f;
	kf->P[0][1] = 0.0f;
	kf->P[1][0] = 0.0f;
	kf->P[1][1] = 1.0f;

	kf->Q[0][0] = 0.1f;
	kf->Q[0][1] = 0.0f;
	kf->Q[1][0] = 0.0f;
	kf->Q[1][1] = 0.1f;

	kf->R = 1.0f;

	kf->K[0] = 0.0f;
	kf->K[1] = 0.0f;

}

void predict(KalmanFilter2D* kf) {
	float x0 = kf->x[0];
	float x1 = kf->x[1];

	// x = F x
	kf->x[0] = kf->F[0][0] * x0 + kf->F[0][1] * x1;
	kf->x[1] = kf->F[1][0] * x0 + kf->F[1][1] * x1;

	// P = F P F^T + Q
	float p00 = kf->P[0][0];
	float p01 = kf->P[0][1];
	float p10 = kf->P[1][0];
	float p11 = kf->P[1][1];

	float fp00 = kf->F[0][0] * p00 + kf->F[0][1] * p10;
	float fp01 = kf->F[0][0] * p01 + kf->F[0][1] * p11;
	float fp10 = kf->F[1][0] * p00 + kf->F[1][1] * p10;
	float fp11 = kf->F[1][0] * p01 + kf->F[1][1] * p11;

	kf->P[0][0] = fp00 * kf->F[0][0] + fp01 * kf->F[0][1] + kf->Q[0][0];
	kf->P[0][1] = fp00 * kf->F[1][0] + fp01 * kf->F[1][1] + kf->Q[0][1];
	kf->P[1][0] = fp10 * kf->F[0][0] + fp11 * kf->F[0][1] + kf->Q[1][0];
	kf->P[1][1] = fp10 * kf->F[1][0] + fp11 * kf->F[1][1] + kf->Q[1][1];
}

void update(KalmanFilter2D* kf, float measurementRange) {
	// y = z - Hx
	float predictedMeasurement = kf->H[0] * kf->x[0] + kf->H[1] * kf->x[1];
	float y = measurementRange - predictedMeasurement;

	// S = H P H^T + R
	float S =
		kf->H[0] * (kf->P[0][0] * kf->H[0] + kf->P[0][1] * kf->H[1]) +
		kf->H[1] * (kf->P[1][0] * kf->H[0] + kf->P[1][1] * kf->H[1]) +
		kf->R;

	// K = P H^T / S
	kf->K[0] = (kf->P[0][0] * kf->H[0] + kf->P[0][1] * kf->H[1]) / S;
	kf->K[1] = (kf->P[1][0] * kf->H[0] + kf->P[1][1] * kf->H[1]) / S;

	// x = x + K y
	kf->x[0] = kf->x[0] + kf->K[0] * y;
	kf->x[1] = kf->x[1] + kf->K[1] * y;

	// P = (I - K H) P
	float p00 = kf->P[0][0];
	float p01 = kf->P[0][1];
	float p10 = kf->P[1][0];
	float p11 = kf->P[1][1];

	kf->P[0][0] = (1.0f - kf->K[0] * kf->H[0]) * p00 - kf->K[0] * kf->H[1] * p10;
	kf->P[0][1] = (1.0f - kf->K[0] * kf->H[0]) * p01 - kf->K[0] * kf->H[1] * p11;
	kf->P[1][0] = -kf->K[1] * kf->H[0] * p00 + (1.0f - kf->K[1] * kf->H[1]) * p10;
	kf->P[1][1] = -kf->K[1] * kf->H[0] * p01 + (1.0f - kf->K[1] * kf->H[1]) * p11;
}

int main() {
	float measurements[10] = {
		100.0f,
		106.0f,
		109.0f,
		116.0f,
		119.0f,
		126.0f,
		131.0f,
		136.0f,
		139.0f,
		146.0f
	};

	float dt = 1.0f;

	KalmanFilter2D kf;
	initKalman(&kf, measurements[0], 0.0f, dt);

	printf("index Measurement Estimated Range Estimated Velocity\n");

	for (int i = 0; i < 10; i++) {
		predict(&kf);
		update(&kf, measurements[i]);

		printf("%d %10.2f %15.2f%18.2f\n", //format specifier(출력형식) 
			i,
			measurements[i],
			kf.x[0],
			kf.x[1]);
	}

	return 0;


}
