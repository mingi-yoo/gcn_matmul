#include "gcn.h"

using namespace std;

uint64_t X[MAX_DIM];
uint64_t W[MAX_DIM];
uint64_t XW[MAX_DIM];
uint64_t mat_info[6]; // a_h, a_w, x_h, x_w, w_h, w_w

void Combination(int h);

int main(int argc, char** argv) {
	// input file
	ifstream ini_file(argv[1]);
	ifstream a_file(argv[2]);
	ifstream x_file(argv[3]);
	ifstream w_file(argv[4]);

	// output file
	ofstream xw_file("xw_matrix.txt");
	ofstream axw_file("axw_matrix.txt");

	if (ini_file.is_open() && x_file.is_open() && w_file.is_open()) {
		string line, tmp;
		// parsing matrix information
		for (int i = 0; i < 6; i++) {
			getline(ini_file, line);
			mat_info[i] = stoull(line);
		}
		// parsing weight matrix
		for (int i = 0; i < mat_info[w_h]; i++) {
			getline(w_file, line);
			stringstream ss(line);
			for (int j = 0; j < mat_info[w_w]; j++) {
				getline(ss, tmp, ' ');
				W[j*mat_info[w_h]+i] = stoull(tmp);
			}
		}

		uint64_t idx = 0;
		uint64_t limit_idx;
		while (idx < mat_info[x_h] - 1) {
			if (idx + MAX_IDX < mat_info[x_h])
				limit_idx = idx + MAX_IDX;
			else
				limit_idx = mat_info[x_h];
			// parsing x matrix
			for (int i = 0; i < limit_idx - idx; i++) {
				getline(x_file, line);
				stringstream ss(line);
				for (int j = 0; j < mat_info[x_w]; j++) {
					getline(ss, tmp, ' ');
					X[i*mat_info[x_w]+j] = stoull(tmp);
				}
			}
			// return matrix multiplication result
			Combination(limit_idx - idx);

			for (int i = 0; i < limit_idx - idx; i++) {
				for (int j = 0; j < mat_info[x_w]; j++) {
					xw_file<<XW[i*mat_info[x_w] + j];
					if (j != mat_info[x_w]-1)
						xw_file<<" ";
					else
						xw_file<<endl;
				}
			}
			idx += limit_idx;
		}
		xw_file.close();
	}
}

void Combination(int h) {
	uint64_t total;
	#pragma omp parallel for private(total) schedule(static) num_threads(16)
	for (int i = 0; i < h; i ++) {
		for(int j = 0; j < mat_info[x_w]; j++) {
			total = 0;
			for (int k = 0; k < mat_info[w_h]; k++)
				total += X[i*mat_info[x_w] + k] * W[i*mat_info[x_w] + k];
			XW[i*mat_info[x_w] + j] = total;
		}
	}
}