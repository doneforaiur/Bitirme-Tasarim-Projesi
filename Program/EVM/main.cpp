#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <valarray>

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


#define PI 3.14159265358979


using std::cout;
using std::string;
using std::tuple;
using std::vector;
using cv::Mat;

using namespace std;

tuple<double*, double*> butter(const double&, const double&);
void fft(CArray &x);


int main(int argc, const char* argv[]) {

	double Afrekans =  1.5;
	double Yfrekans =  0.5;
	int seviye = 4;
	float alfa = 100.0;
	
	string default_args[] = { "0.5", "1.5", "4", "300" };
	string config;
	fstream configs("config.txt");

	cv::VideoCapture cap;
	getline(configs, config);
	configs.close();
	if (config == "kamera") {
		cap.open(0, 0);
		cap.set(cv::CAP_PROP_FPS, 30);
	}
	else {
		cap.open(config);
	}


	int frame_c = 292; //cap.get(cv::CAP_PROP_FRAME_COUNT);
	Complex test[293] = { 0.0 };

	double fs = cap.get(cv::CAP_PROP_FPS);

	auto[low_a, low_b] = butter(Afrekans, fs);
	auto[high_a, high_b] = butter(Yfrekans, fs);

	int level = seviye;

	vector<Mat>* data = new vector<Mat>;
	vector<vector<Mat>> pyramid;
	pyramid.resize(level);


	int frame_counter = 0;
	int fourier_counter = 0;

	Mat frame;
	while (true) {

		cap >> frame;

		Mat rgbchannel[3];
		split(frame, rgbchannel);
		//frame = (rgbchannel[0] + rgbchannel[1] + rgbchannel[2])/3.0;
		frame = rgbchannel[1];

		if (frame.empty()) {
			return 0;
		}

		if (frame_counter > 9) {
			frame_counter = 0;

			fstream configs("config.txt");
			
			getline(configs, config);

			int p = 0;
			while (getline(configs, config)) {
				default_args[p] = string(config);
				p++;
			}

			configs.close();
			Afrekans = stod(default_args[0]);
			Yfrekans = stod(default_args[1]);

			alfa = stof(default_args[2]);
			seviye = stoi(default_args[3]);

			auto[low_a, low_b] = butter(Afrekans, fs);
			auto[high_a, high_b] = butter(Yfrekans, fs);


		}
		frame_counter++;

		

		frame.convertTo(frame, CV_64F, 1.0 / 255.0f);
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

				Mat m = alfa * (lowpass2 - lowpass1);

				if (i == level - 1) {
					Mat m_;
					Mat temp;
					for (int l = 0; l < level - 1; l++) {
						cv::pyrUp(m, temp);
						m = temp;
					}

					
					if (fourier_counter < 293) {
						test[fourier_counter] = mean(m)[0];//(m.at<double>(m.rows/2, m.cols/2));
						cout << fixed  <<  mean(m)[0] << "\n";//(m.at<double>(m.rows/2, m.cols/2)) << "\n";
						//cout << fourier_counter  << "," << mean(m)[1] << "\n";
					}

					//cv::Rect rect(frame.cols/2-5, frame.rows/2-5, 10, 10);
					//cv::rectangle(frame, rect, cv::Scalar(0, 0, 0));

					imshow("Çıkış", frame + m);
					imshow("Maske", m);
					cv::waitKey(1);

					if (fourier_counter == 292) {
						cout << "fft data";
						CArray data1(test, 293);
						fft(data1);
						for (int i = 0; i < 294; i++)
						{
							//cout << i << "######\n";
							cout <<i << " , " << abs(data1[i]) << "\n";
							//cout << i << ","<< atan2(data1[i].imag(), data1[i].real()) << "\n";
						}

						return 0;
					}
					fourier_counter++;
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

void fft(CArray& x)
{
	const size_t N = x.size();
	if (N <= 1) return;

	CArray cift = x[std::slice(0, N / 2, 2)];
	CArray  tek = x[std::slice(1, N / 2, 2)];

	fft(cift);
	fft(tek);

	for (size_t k = 0; k < N / 2; ++k)
	{
		Complex t = std::polar(1.0, -2 * PI * k / N) * tek[k];
		x[k] = cift[k] + t;
		x[k + N / 2] = cift[k] - t;
	}
}
