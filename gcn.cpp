#include "gcn.h"

using namespace std;

int X[MAX_DIM];
int W[MAX_DIM];
int XW[MAX_DIM];
int a_h, a_w, x_h, x_w, w_h, w_w;

vector<int> a_row;
vector<int> a_col;
vector<vector<int>> xw_row;
vector<vector<int>> axw_row;

void Combination(int h);
void Aggregation(int h);

int main(int argc, char** argv) {
	// input file
	ifstream ini_file(argv[1]);
	ifstream a_file(argv[2]);
	ifstream x_file(argv[3]);
	ifstream w_file(argv[4]);

	// output file
	ofstream xw_file("xw_matrix.txt");
	ofstream axw_file("axw_matrix.txt");

	if (ini_file.is_open() && a_file.is_open() && x_file.is_open() && w_file.is_open()) {
		string line, tmp;
		// parsing matrix information
		for (int i = 0; i < 6; i++) {
			getline(ini_file, line);
			if (i == 0)
				a_h = stoi(line);
			else if (i == 1)
				a_w = stoi(line);
			else if (i == 2)
				x_h = stoi(line);
			else if (i == 3)
				x_w = stoi(line);
			else if (i == 4)
				w_h = stoi(line);
			else
				w_w = stoi(line);
		}

		for (int i = 0; i < x_h; i++) 
			xw_row.push_back(vector<int> ());
		for (int i = 0; i < MAX_IDX; i++)
			axw_row.push_back(vector<int> ());
		// parsing weight matrix
		for (int i = 0; i < w_h; i++) {
			getline(w_file, line);
			stringstream ss(line);
			for (int j = 0; j < w_w; j++) {
				getline(ss, tmp, ' ');
				W[j * w_h + i] = stoi(tmp);
			}
		}

		int idx = 0;
		int limit_idx;
		while (idx < x_h) {
			if (idx + MAX_IDX < x_h)
				limit_idx = idx + MAX_IDX;
			else
				limit_idx = x_h;
			// parsing x matrix
			for (int i = 0; i < limit_idx - idx; i++) {
				getline(x_file, line);
				stringstream ss(line);
				for (int j = 0; j < x_w; j++) {
					getline(ss, tmp, ' ');
					X[i * x_w + j] = stoi(tmp);
				}
			}
			// return matrix multiplication result
			Combination(limit_idx - idx);
			// make xw matrix text file
			for (int i = 0; i < limit_idx - idx; i++) {
				for (int j = 0; j < x_w; j++) {
					xw_file<<XW[i * x_w + j];
					xw_row[idx+i].push_back(XW[i*x_w + j]);
					if (j != x_w-1)
						xw_file<<" ";
					else
						xw_file<<endl;
				}
			}
			idx = limit_idx;
		}
		xw_file.close();

		getline(a_file, line);
		stringstream ss(line);
		while (getline(ss, tmp, ' ')) {
			if (tmp == "\n")
				break;
			a_row.push_back(stoi(tmp));
		}
		uint max_col = a_row.back();

		getline(a_file, line);
		ss.clear();
		ss.str(line);
		for (int i = 0; i < max_col; i++) {
			getline(ss, tmp, ' ');
			a_col.push_back(stoi(tmp))
		}

		// aggregation
		idx = 0;
		while (idx < a_h) {
			if (idx + MAX_IDX < a_h)
				limit_idx = idx + MAX_IDX;
			else
				limit_idx = a_h;
			Aggregation(limit_idx - idx);
			for (int i = 0; i < limit_idx - idx; i++) {
				for (int j = 0; j < w_w; j++) {
					axw_file<<axw_row[i][j];
					if (j != w_w - 1)
						axw_file<<" ";
					else
						axw_file<<endl;
				}
				axw_row[i].clear();
			}
			idx = limit_idx;
		}
		axw_file.close();
	}
}

// this is combination function
void Combination(int h) {
	int total;

	#pragma omp parallel for private(total) schedule(static) num_threads(16)
	for (int i = 0; i < h; i ++) {
		for(int j = 0; j < x_w; j++) {
			total = 0;
			for (int k = 0; k < w_h; k++)
				total += X[i * x_w + k] * W[j * x_w + k];
			XW[i * x_w + j] = total;
		}
	}
}

// this is aggregation function
void Aggregation(int h) {
	int max_edge;
	int accum_edge;

	#pragma omp parallel for private(max_edge, accum_edge) schedule(static) num_threads(16)
	for (int i = 0; i < h; i++) {
		max_edge = a_row[i+1] - a_row[i];
		accum_edge = a_row[i];
		for (int j = 0; j < max_edge; j++) {
			int e = a_col[accum_edge + j];
			if (j == 0) {
				for (int k = 0; k < x_w; k++)
					axw_row[i].push_back(xw_row[e][k]);
			}
			else {
				for (int k = 0; k < x_w; k++)
					axw_row[i][k] += xw_row[e][k];
			}
		}
	}

	accum_edge = a_row[h];
	for (int i = 0; i < h; i++) {
		a_row.erase(a_row.begin());
	}
	for (int i = 0; i < accum_edge; i++) {
		a_col.erase(a_col.begin());
	}
}