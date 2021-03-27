#include <iostream>
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

using namespace std::chrono;

tuple<double* , double*> butter(const double&, const double&);

int main(int argc, const char* argv[]) {

	double Afrekans;
	double Yfrekans;
	int seviye;
	int alfa;
	string dosya_yolu;

	int const default_argc = 5;
	const char* default_args[] = { "0.5", "1.5", "3", "50", "face.mp4" };
	if (argc == 1) {

		argc = default_argc;
		argv = default_args;

		Afrekans = std::stod(argv[0]);
		Yfrekans = std::stod(argv[1]);
		seviye = std::stoi(argv[2]);
		alfa = std::stoi(argv[3]);
		dosya_yolu = argv[4];

	}
	else {
		cout << argv[0];
		Afrekans = std::stod(argv[1]);
		Yfrekans = std::stod(argv[2]);
		seviye = std::stoi(argv[3]);
		alfa = std::stoi(argv[4]);
		dosya_yolu = argv[5];
	}

	cout.precision(8);
	auto [low_a, low_b] = butter(0.5, 30.0);
	cout << low_a[0] << " " << low_a[1] << "\n";
	cout << low_b[0] << " " << low_b[1] << "\n";
	auto [high_a, high_b] = butter(1.5, 30.0);
	cout << high_a[0] << " " << high_a[1] << "\n";
	cout << high_b[0] << " " << high_b[1] << "\n";


	cv::VideoCapture cap("test4.mp4");
	double fs = cap.get(cv::CAP_PROP_FPS);

	const int level = seviye;


	vector<Mat>* data = new vector<Mat>;
	vector<vector<Mat>> pyramid;
	pyramid.resize(level);

	Mat frame;
	while (true) {
		cap >> frame;
		if (frame.empty()) {
			cout << "boþ frame\n";
			break;
		}
		if (!cap.isOpened()) {
			cout << "ERROR! Unable to open camera\n";
			return -1;
		}
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

				lowpass1 = (-high_b[1] * pyramid[i][0] + high_a[0] * pyramid[i][3] + high_a[1] * pyramid[i][2]) / high_b[0];
				lowpass2 = (-low_b[1] * pyramid[i][1] + low_a[0] * pyramid[i][3] + low_a[1] * pyramid[i][2]) / low_b[0];
				Mat m = alfa * (lowpass1 - lowpass2);

				if (i == 2) {

					Mat m_;
					Mat temp;

					for (int l = 0; l < level - 1; l++) {
						cv::pyrUp(m, m);
					}

					imshow("b", frame + m);
					imshow("a", frame);
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

	static double low_a[2] = {0.0}, low_b[2] = {0.0};
	double th = (double)(2.0 * PI * fc / fs);
	double g = (cos(th) / (1.0 + sin(th)));
	low_a[0] = (double)(1.0 - g) / 2.0;
	low_a[1] = (1.0 - g) / 2.0;
	low_b[0] = 1.0;
	low_b[1] = -1.0 * g;

	return { low_a,low_b };
}