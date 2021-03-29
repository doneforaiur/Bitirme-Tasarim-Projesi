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


	//const Complex test[] = { 2.0, 2.0, 2.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
	//CArray data1(test, 8);

	//fft(data1);

	//for (int i = 0; i < 8; ++i)
	//{
	//	cout << std::abs(data1[i]) << "\n";
	//	cout << atan2(data1[i].imag(), data1[i].real()) << "\n";
	//}





	double Afrekans =  1.5;
	double Yfrekans =  0.5;
	int seviye = 4;
	float alfa = 100.0;
	
	string default_args[] = { "0.5", "1.5", "4", "300" };
	
	string config;

	//fstream configs("config.txt");

	cv::VideoCapture cap;
	//getline(configs, config);
	//configs.close();
	//if (config == "kamera") {
		cap.open(0, 0);
		cap.set(cv::CAP_PROP_FPS, 30);
	//}
	//else {
	//	cap.open(config);
	//}


	double fs = cap.get(cv::CAP_PROP_FPS);

	auto[low_a, low_b] = butter(Afrekans, fs);
	auto[high_a, high_b] = butter(Yfrekans, fs);

	int level = seviye;

	vector<Mat>* data = new vector<Mat>;
	vector<vector<Mat>> pyramid;
	pyramid.resize(level);


	int frame_counter = 0;

	Mat frame;
	while (true) {
		cap >> frame;
		
		//Mat rgbchannel[3];
		//split(frame, rgbchannel);
		//frame = rgbchannel[2];

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
					imshow("Çıkış", frame + m);
					imshow("Maske", m);
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


void fft(CArray &x)
{
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				Complex t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}

	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
}
