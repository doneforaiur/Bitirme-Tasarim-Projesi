#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <valarray>
#include "opencv2/highgui.hpp"

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

	string default_args[] = {"kamera", "0.5", "1.5", "300", "2", "false", "-1" };
	string dosya_yolu, config;
	
	fstream configs("config.txt");

	getline(configs, dosya_yolu); // dosya yolunu geçmek için
	int p = 0;
	while (getline(configs, config)) {
		default_args[p] = string(config);
		p++;
	}

	configs.close();
	double Afrekans = stod(default_args[0]);
	double Yfrekans = stod(default_args[1]);

	float alfa = stof(default_args[2]);
	int seviye = stoi(default_args[3]);

	bool gelismis = (default_args[4] == "true" ? true : false);
	int  performans = stoi(default_args[5]);
	
	int frame_c = 256; // ftt için default
	CArray test(256);

	cv::VideoCapture cap;

	configs.close();
	if (dosya_yolu == "kamera") {
		cap.open(0, 0);
		cap.set(cv::CAP_PROP_FPS, 30); // kameranın fps'i ne olursa olsun
	}								   // 30'a setliyor
	else {
		cap.open(dosya_yolu);
		frame_c = cap.get(cv::CAP_PROP_FRAME_COUNT);  
		test = CArray(256);
	}

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

	cap >> frame;

	cv::Size s = frame.size();

	while (true) {
		cap >> frame;
		if (performans > -1) {
			Mat rgbchannel[3];
			split(frame, rgbchannel);
			frame = rgbchannel[performans];
		}

		if (frame.empty()) {
			return 0;
		}

		frame_counter++;

		frame.convertTo(frame, CV_64F, 1.0 / 255.0);
		Mat current = frame;

		Mat laplace;

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

			
				if ((i == level - 2)  && (gelismis==true)) {
					Mat temp;
					laplace = m;
					for (int l = 0; l < level - 2; l++) {
						cv::pyrUp(laplace, temp);
						laplace = temp;
					}
				}


				if (i == level - 1) {
					Mat m_, temp;
					
					for (int l = 0; l < level - 1; l++) {
						cv::pyrUp(m, temp);
						m = temp;
					}


					Mat laplace_temp;
					Mat binaryMat;
					
					if (gelismis == true) {
						laplace.convertTo(laplace_temp, CV_8U, 255, 0);
						m.copyTo(binaryMat, laplace_temp);
						m = binaryMat;
					}
					imshow("Orijinal", frame);
					imshow("Çıkış",  frame + m);
					cv::waitKey(1);
					 
					if (fourier_counter < 256) {
						test[fourier_counter] = mean(m)[0];
					}

					if (fourier_counter > 256) {
						test = test.shift(1);
						test[255] = mean(m)[0];
					}
					ofstream data1;
					data1.open("data.txt");
					for (int i = 0; i < test.size(); i++) {
						data1 << test[i].real() << endl;
					}

					fourier_counter++;

					if (fourier_counter > 10) {
						
						vector<double> data;

						CArray test_temp(256);
						test_temp = test;
						fft(test_temp);

						for (int i = 0; i < test_temp.size()/2; i++)
						{
							data.push_back(abs(test_temp[i])); // ## GÜÇ^2
							// abs(test[i])/255.0							    ## GÜÇ
							// atan2(data1[i].imag(), data1[i].real())			## FAZ						
						}

						ofstream fourier;
						fourier.open("fourier.txt");
						for (int i = 0; i < test_temp.size() / 2; i++) {
							fourier << abs(test_temp[i]) << endl;

						}

						int Abin = floor(Afrekans / (fs / 256.0));
						int Ybin = ceil(Yfrekans / (fs / 256.0));
						int maxElementIndex = std::max_element(data.begin()+Abin, data.begin()+Ybin) - data.begin();
					
						float Anabiz = (fs / 256.0 * (maxElementIndex + 1) * 60.0) - (60.0 * fs / 256.0) / 4;
						float Ynabiz = (fs / 256.0 * (maxElementIndex + 1) * 60.0) + (60.0 * fs / 256.0) / 4;
						cout  << fixed << setprecision(1) << Anabiz << " " << Ynabiz << endl;
						ofstream nabiz;
						nabiz.open("nabız.txt");
						nabiz << fixed << setprecision(1) << Anabiz << "-" << Ynabiz << endl;
						nabiz.close();
					}
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

