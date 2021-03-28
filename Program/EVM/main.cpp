#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>
#include<math.h>

#define PI 3.14159265358979

using std::cout;
using std::string;
using std::tuple;
using std::vector;
using cv::Mat;

using namespace std;

tuple<double*, double*> butter(const double&, const double&);

int main(int argc, const char* argv[]) {

	double Afrekans;
	double Yfrekans;
	int seviye;
	int alfa;
	
	string default_args[] = { "0.5", "1.5", "4", "300" };
	
	string config;

	fstream configs("config.txt");

	cv::VideoCapture cap;
	getline(configs, config);
	if (config == "kamera") {
		cap.open(0, 0);
		cap.set(cv::CAP_PROP_FPS, 30);
	}
	else {
		cap.open(config);
	}


	double fs = cap.get(cv::CAP_PROP_FPS);

	auto[low_a, low_b] = butter(0.5, fs);
	auto[high_a, high_b] = butter(1.5, fs);

	const int level = seviye;

	vector<Mat>* data = new vector<Mat>;
	vector<vector<Mat>> pyramid;
	pyramid.resize(level);


	int frame_counter = 0;

	Mat frame;
	while (true) {

		if (frame.empty()) {
			return 0;
		}

		if (frame_counter > 9) {
			frame_counter == 0;

			int p = 0;
			while (getline(configs, config)) {
				default_args[p] = string(config);
				p++;
			}
			Afrekans = stod(default_args[0]);
			Yfrekans = stod(default_args[1]);
			seviye = stoi(default_args[2]);
			alfa = stoi(default_args[3]);
		}

		frame_counter++;

		cap >> frame;
		
		frame.convertTo(frame, CV_64F, 1.0 / 255.0f);

		//data->push_back(frame);

		Mat current = frame;

		for (int i = 0; i < level; i++) {


			Mat down, up;


			if (pyramid[i].size() == 3) {

				pyramid[i][0] = current;
				pyramid[i][1] = current;
			}

			if (pyramid[i].size() > 3) {


				Mat lowpass1, lowpass2;

				lowpass1 = (-high_b[1] * pyramid[i][0] + high_a[0] * pyramid[i][3] + high_a[0] * pyramid[i][2]) / high_b[0];
				lowpass2 = (-low_b[1] * pyramid[i][1] + low_a[0] * pyramid[i][3] + low_a[0] * pyramid[i][2]) / low_b[0];
				Mat m = alfa * (lowpass1 - lowpass2);

				if (i == level - 1) {

					Mat m_;
					Mat temp;

					for (int l = 0; l < level - 1; l++) {
						cv::pyrUp(m, temp);
						m = temp;
					}

					imshow("b", frame + m);
					cv::waitKey(1);
				}

				pyramid[i][0] = lowpass1;
				pyramid[i][1] = lowpass2;

				if (pyramid[i].size() > 3)
					pyramid[i].erase(pyramid[i].begin() + 2);

			}

			if (i == (level - 1)) {
				pyramid[i].push_back(current);
				break;
			}

			cv::pyrDown(current, down);
			cv::pyrUp(down, up, current.size());
			pyramid[i].push_back(current - up);
			current = down;

		}
	}
}

tuple<double *, double *> butter(const double &fc, const double &fs) {

	double* low_a = { new double[2]{0.0} };
	double* low_b = { new double[2]{0.0} };
	double th = 2.0 * PI * fc / fs;
	double g = cos(th) / (1.0 + sin(th));
	low_a[0] = (1.0 - g) / 2.0;
	low_a[1] = (1.0 - g) / 2.0;
	low_b[0] = 1.0;
	low_b[1] = -g;

	return { low_a,low_b };
}
