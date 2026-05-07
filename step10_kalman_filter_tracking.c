#include <stdio.h>

typedef struct {
	float x;  // estimated range
	float P;  // estimation error
	float Q;  // process noise
	float R;  // measurement noise
	float K;  // Kalman gain
} KalmanFilter;

void kalmanInit(KalmanFilter* kf, float initValue) {
	kf -> x = initValue;
	kf->P = 1.0f;
	kf->Q = 0.1f;
	kf->R = 1.0f;
	kf->K = 0.0f;
}

float kalmanUpdate(KalmanFilter* kf, float measurement) {

	// prediction
	kf->P = kf->P + kf->Q;

	// Kalman Gain
	kf->K = kf->P / (kf->P + kf->R);

	// update estimate
	kf->x = kf->x + kf->K * (measurement - kf->x);

	// update error covariance
	kf->P = (1 - kf->K) * kf->P;

	return kf->x;
}

int main() {

	float measurements[10] = {
		100.0f,
		102.0f,
		101.0f,
		105.0f,
		107.0f,
		106.0f,
		108.0f,
		110.0f,
		111.0f,
		109.0f
	};

	KalmanFilter kf;

	kalmanInit(&kf, measurements[0]);

	int i;

	printf("Measurement      Filter\n");

	for (i = 0; i < 10; i++) {
		
		float filtered = kalmanUpdate(&kf, measurements[i]);

		printf("%8.2f       %8.2f\n",
			measurements[i],
			filtered);
	}

	return 0;

}